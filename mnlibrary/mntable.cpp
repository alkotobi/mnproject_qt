#include "mntable.h"
#include "mnexception.h"
#include <iostream>




bool MnTable::exec(const QString& sql, const QList<QVariant>& params)
{
    if(conn != nullptr && sql!=""){
         return conn->exec(sql,params);
    }else
    {
        return false;
    }
}


MnTable::MnTable(mnconnection *conn, MnTableDef table, QObject *parent)
:MnCustomQry(parent),_sql(table.selectSql())
{
    this->conn=conn;
    this->tableDef = table;

}

void MnTable::close() {
    if (!fActive) return;
    data.clear();
    fActive= false;
}

int MnTable::recordCount() const {
    return fRecordCount;
}

bool MnTable::open(QList<QVariant> params) {
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

void MnTable::edit() {
    fState = stEdit;
}

bool MnTable::append() {
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

bool MnTable::post() {
    bool res = false;
    if(data[row][0] == ""){
       //do insert
        res = this->conn->exec(this->conn->insertSql(_sql.tableName(),_sql.insertFields().join(",")));

    }else{
       //do update
      res = this->conn->exec(this->conn->updateSql(_sql.tableName(),_sql.insertFields().join(","),"id="+data[row][0]), toVariants(_sql.insertFields()));
    }
    fState=stBrowse;
    return res;
}

bool MnTable::goTo(int ind) {
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

bool MnTable::execBeforeScroll() {
    bool ret= true;
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        ret=ret && beforeScrollNtfs[i](this);
    }
    return ret;
}

void MnTable::execAfterScroll() {
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        afterScrollNtfs[i](this);
    }
}

void MnTable::addDataSource(MnCustomDataSource* dts)
{
    if(dataSources.contains(dts)) return;
    dataSources.append(dts);
}

void MnTable::removeDataSource(MnCustomDataSource *dts)
{
    dataSources.removeOne(dts);
}

QString *MnTable::fieldByName(const QString &name)
{
    if (name == "id"){
        return &data[row][0];
    }
    int index =this->fieldIndex(name);
    if(index < 0 ) throw MNException(name + "is not a correct field name");
    return &(data[this->row][index]);
}

QString *MnTable::fieldByInd(const int index)
{
    return &(data[this->row][index]);
}

bool MnTable::next() {
    if (row != data.count() - 1){
        goTo(row + 1);
        return true;
    }
    return false;
}

bool MnTable::prior() {
    if (row != 0){
        goTo(row - 1);
        return true;
    }
    return false;
}

bool MnTable::last() {
    int i =data.count()-1;
    if (row != i){
        goTo(i);
        return true;
    }
    return false;
}

bool MnTable::first() {
    if (row != 0){
        goTo(0);
        return true;
    }
    return false;
}



MnTable::MnTable(mnconnection *conn, QString sql, QObject *parent)
: MnCustomQry(parent),_sql(sql){
    tableDef = conn->tableDef(_sql.tableName(),_sql.fields());
    this->conn = conn;
}

QString MnTable::sqlText() {
     return _sql.text();
}

int MnTable::fieldIndex(const QString& fieldName) {
    return (int)_sql.fields().indexOf(fieldName);
}

void MnTable::printAll() {
    for (const QStringList& row : data) {
        for (const QString& item : row) {
            std::cout << item.toStdString() << "\t";
        }
        std::cout << "\n";
    }
}

void MnTable::printTableDef() {
    tableDef.print();

}

void MnTable::printCurrent() {
    for (const QString& item : data[row]) {
        std::cout << item.toStdString() << "\t";
    }
    std::cout << "\n";
}

QList<QVariant> MnTable::toVariants(const QStringList& fields) {
    QList<QVariant> params = {};
    for (const QString& fld : fields) {
        //auto f = tableDef.fieldByName(fld);
       auto col= tableDef.fieldIndex(fld);
        auto f = tableDef.fields[col];
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

QStringList MnTable::rowAt(int row) {
    return data[row];
}

int MnTable::rowNo() {
    return row;
}




