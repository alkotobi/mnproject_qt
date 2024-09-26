#include "mnqry.h"
#include "mnexception.h"
#include <iostream>




bool MnQry::exec(const QString& sql, const QList<QVariant>& params)
{
    if(conn != nullptr && sql!=""){
         return conn->exec(sql,params);
    }else
    {
        return false;
    }
}


MnQry::MnQry(mnconnection *conn, MnTableDef table, QObject *parent)
:MnCustomQry(parent),_sql(table.selectSql())
{
    this->conn=conn;
    this->tableDef = table;

}

void MnQry::close() {
    if (!fActive) return;
    data.clear();
    fActive= false;
}

int MnQry::recordCount() const {
    return fRecordCount;
}

bool MnQry::open(QList<QVariant> params) {
    if(fActive){
        throw MNException("can not perform this operation on an open qry");
    }
    QStringList fld ={};
    QStringList* fldPtr = &fld;
    QString s =sqlText();
    if(_sql.fieldsContains("*")){
        _sql.fieldsClear();
    }else{
        fldPtr = nullptr;
    }
   bool ret= this->conn->exec(s, params, &data, fldPtr);
    if (fldPtr){
        if(!fld.contains("id")){
            fld.insert(0,"id");
        }
        tableDef = conn->tableDef(_sql.tableName(), fld);
        _sql.fieldAppend(fld);
        _sql.insertFieldsClear();
    }
    fRecordCount = (int)data.count();
    if (ret) {
        fActive = true;
        fState = stBrowse;
    }
    else fActive = false;
   return ret;
}

void MnQry::edit() {
    fState = stEdit;
}

bool MnQry::append() {
    if (fState != stBrowse) post();
    fState=stInsert;
    QStringList l;
    for (int i =0;i<_sql.insertFields().count();i++){
        l.append("");
    }
    data.append(l);
    if(!goTo((data.count()-1))){
        data.removeLast();
        return false;
    }
    return true;
}

bool MnQry::post() {
    if(data[row][0] == ""){
       //do insert
        this->conn->insertSql(_sql.tableName(),_sql.insertFields().join(","));

    }else{
       //do update
      this->conn->exec(this->conn->updateSql(_sql.tableName(),_sql.insertFields().join(","),"id="+data[row][0]), toVariants(_sql.insertFields()));
    }
    fState=stBrowse;
    return false;
}

bool MnQry::goTo(int ind) {
    if (data.empty()) return false;
    if (!execBeforeScroll()) return false;
    if(ind <0 && ind>= data.count()){
        qCritical()<< "index out of range\n";
        return false;
    }
    if(fState != stBrowse){
        if(!post())
            return false;
    }
    this->row = ind;
    execAfterScroll();
    //update controls
    for (int i = 0; i < dataSources.count(); ++i) {
        dataSources[i]->updateControls();
    }
    return true;
}

bool MnQry::execBeforeScroll() {
    bool ret= true;
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        ret=ret && beforeScrollNtfs[i](this);
    }
    return ret;
}

void MnQry::execAfterScroll() {
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        afterScrollNtfs[i](this);
    }
}

void MnQry::addDataSource(MnCustomDataSource* dts)
{
    if(dataSources.contains(dts)) return;
    dataSources.append(dts);
}

void MnQry::removeDataSource(MnCustomDataSource *dts)
{
    dataSources.removeOne(dts);
}

QString *MnQry::fieldByName(const QString &name)
{
    if (name == "id"){
        return &data[row][0];
    }
    int index =this->fieldIndex(name);
    if(index < 0 ) throw MNException(name + "is not a correct field name");
    return &(data[this->row][index]);
}

QString *MnQry::fieldByInd(const int index)
{
    return &(data[this->row][index]);
}

bool MnQry::next() {
    if (row != data.count() - 1){
        goTo(row + 1);
        return true;
    }
    return false;
}

bool MnQry::prior() {
    if (row != 0){
        goTo(row - 1);
        return true;
    }
    return false;
}

bool MnQry::last() {
    int i =data.count()-1;
    if (row != i){
        goTo(i);
        return true;
    }
    return false;
}

bool MnQry::first() {
    if (row != 0){
        goTo(0);
        return true;
    }
    return false;
}



MnQry::MnQry(mnconnection *conn, QString sql, QObject *parent)
: MnCustomQry(parent),_sql(sql){
    tableDef = conn->tableDef(_sql.tableName(),_sql.fields());
    this->conn = conn;
}

QString MnQry::sqlText() {
     return _sql.text();
}

int MnQry::fieldIndex(const QString& fieldName) {
    return (int)_sql.fields().indexOf(fieldName);
}

void MnQry::printAll() {
    for (const QStringList& row : data) {
        for (const QString& item : row) {
            std::cout << item.toStdString() << "\t";
        }
        std::cout << "\n";
    }
}

void MnQry::printTableDef() {
    tableDef.print();

}

void MnQry::printCurrent() {
    for (const QString& item : data[row]) {
        std::cout << item.toStdString() << "\t";
    }
    std::cout << "\n";
}

QList<QVariant> MnQry::toVariants(const QStringList& fields) {
    QList<QVariant> params = {};
    for (const QString& fld : fields) {
        //auto f = tableDef.fieldByName(fld);
       auto col= tableDef.fieldIndex(fld);
        auto f = tableDef.fields[row];
        switch (f.field_type) {

            case INTEGER:
                params.append(data[row][col].toInt());
                break;
            case TEXT:
            case VARCHAR:
                params.append(data[row][col]);
                break;
            case REAL:
                params.append(data[row][col].toDouble());
                break;
            case BLOB:
                //TODO:BLOB
                break;
            case BOOL:
                params.append(data[row][col].toInt()!=0 );
                break;
            case DATETIME:
                params.append(data[row][col] );
                break;
            default:
                throw MNException("field type "+QString::number(f.field_type)+" UNHANDLED");
        }
    }

    return params;
}

QStringList MnQry::rowAt(int row) {
    return data[row];
}

int MnQry::rowNo() {
    return row;
}




