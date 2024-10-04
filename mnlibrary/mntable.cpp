#include "mntable.h"
#include "mnexception.h"
#include <iostream>
#include <utility>
#include "mnview.h"


bool MnTable::exec(const QString &sql, const QList<QVariant> &params) {
    if (conn != nullptr && sql != "") {
        return conn->exec(sql, params);
    } else {
        return false;
    }
}


MnTable::MnTable(mnconnection *conn, MnTableDef table,QString where , QList<QVariant> params, QObject *parent)
        : QObject(parent), sql_(table.selectSql(),where) {
    this->conn = conn;
    this->fTableDef_ = table;
    this->fParams=params;
}

void MnTable::close() {
    if (!fActive) return;
    data()->clear();
    fActive = false;
}

int MnTable::recordCount() {
    return data()->size();
}

bool MnTable::open(QList<QVariant> params) {
    if (fActive) {
        throw MNException("can not perform this operation on an open qry");
    }
    if(!params.isEmpty())
        fParams = params;
    QStringList fld = {};
    QStringList *fldPtr = &fld;
    QString s = sqlText();
    if (fieldsContains("*")) {
        fieldsClear();
    } else {
        fldPtr = nullptr;
    }
    bool ret = this->conn->exec(s, fParams, data(), fldPtr);
    if (!ret){
        return false;
    }
    if (fldPtr) {
        if (!fld.contains("id")) {
            fld.insert(0, "id");
        }
        fieldAppend(fld);
        insertFieldsClear();
    }
    if (fTableDef_.table_name ==""){
        fTableDef_ = conn->tableDef(tableName(), fields());
    }
//    fRecordCount = (int) data()->count();
    if (ret) {
        fActive = true;
        fState = stBrowse;
    } else fActive = false;
    if (!data()->isEmpty())
        row = 0;
    return ret;
}

void MnTable::edit() {
    if (data()->isEmpty()) throw MNException("cant edit an empty dataset");
    fOldVals = (*data())[row];
    fState = stEdit;
}

bool MnTable::append() {
    if (!execBeforeScroll())
        return false;
    if (fState != stBrowse) post();
    fState = stInsert;
    QStringList l;
    for (int i = 0; i < fields().count(); i++) {
        l.append("");
    }
    data()->append(l);
    row = (int) data()->count() - 1;
    execAfterScroll();
    return true;
}

bool MnTable::post() {
    if (!execBeforePost())
        return false;
    bool res = false;
    if (!fNotEdited) {
        if ((*data())[row][0] == "") {
            //do insert
            QStringList insFlds = insertFields();
            int id = this->conn->execInsertSql(tableName(), insFlds.join(","),
                                               toVariants(insFlds));
            setFieldValue(0, QString::number(id));
            res = id != -1;

        } else {
            //do update
            QStringList insFlds = insertFields();
            res = this->conn->execUpdateSql(
                    tableName(), insFlds.join(","), "id=" + (*data())[row][0],
                    toVariants(insFlds));
        }
        fNotEdited = true;
    }

    fOldVals.clear();
    fState = stBrowse;
    execBeforePost();
    return res;
}

bool MnTable::goToNoBoundChenck(int ind) {
    if (data()->empty()) return false;
    if (!execBeforeScroll()) return false;
    if (fState != stBrowse) {
        if (fState == stInsert && fNotEdited) {
            cancel();
        } else if (!post())
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

bool MnTable::goTo(int ind) {
    if (ind < 0 && ind >= data()->count()) {
        throw MNException("index out of range\n");
    }
    return goToNoBoundChenck(ind);
}

bool MnTable::execBeforeScroll() {
    bool ret = true;
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        ret = ret && beforeScrollNtfs[i](this);
    }
    return ret;
}

void MnTable::execAfterScroll() {
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        afterScrollNtfs[i](this);
    }
}

void MnTable::addDataSource(MnCustomDataSource *dts) {
    if (dataSources.contains(dts)) return;
    dataSources.append(dts);
}

void MnTable::removeDataSource(MnCustomDataSource *dts) {
    dataSources.removeOne(dts);
}

QString MnTable::fieldByName(const QString &name) const {
    if (name == "id") {
        return (*data())[row][0];
    }
    int index = this->fieldIndex(name);
    if (index < 0) throw MNException(name + "is not a correct field name");
    return ((*data())[this->row][index]);
}

QString MnTable::fieldByInd(const int index) {
    return ((*data())[this->row][index]);
}

void MnTable::setFieldValue(const QString &fieldName, const QString &val) {

    int index = this->fieldIndex(fieldName);
    setFieldValue(index, val);
}

void MnTable::setFieldValue(int col, const QString &val) {
    if (fState == stBrowse) throw MNException("Dataset not in edit or insert mode");
    if (col < 0 || col > fields().size()) throw MNException(QString::number(col) + "is not a correct field col");
    if ((*data())[this->row][col] == val) return;
    QString s = val;
    if (!doBeforeSetFieldVal(this, (*data())[this->row][col], s)) {
        return;
    }
    (*data())[this->row][col] = s;
    fNotEdited = false;
}

bool MnTable::next() {
    if (data()->isEmpty()) return false;
    if (row != data()->count() - 1) {
        return goTo(row + 1);
    }
    return false;
}

bool MnTable::prior() {
    if (data()->isEmpty()) return false;
    if (row != 0) {
        return goTo(row - 1);
    }
    return false;
}

bool MnTable::last() {
    if (data()->isEmpty()) return false;
    int i = (int) data()->count() - 1;
    if (row != i) {
        return goTo(i);
    }
    return false;
}

bool MnTable::first() {
    if (data()->isEmpty()) return false;
    if (row != 0) {
        return goTo(0);
    }
    return false;
}


MnTable::MnTable(mnconnection *conn, QString sql, QList<QVariant> params,QObject *parent)
        : QObject(parent), sql_(sql) {
    //fTableDef = conn->tableDef(_sql.tableName(), _sql.fields());
    this->conn = conn;
    this->fParams = params;
}

QString MnTable::sqlText() {
    return sql_.text();
}

int MnTable::fieldIndex(const QString &fieldName)const {
    return (int) sql_.fields().indexOf(fieldName);
}

void MnTable::printAll() {
    for (const QStringList &row: (*data())) {
        for (const QString &item: row) {
            std::cout << item.toStdString() << "\t";
        }
        std::cout << "\n";
    }
}

void MnTable::printTableDef() {
    fTableDef_.print();

}

void MnTable::printCurrent() {
    for (const QString &item: (*data())[row]) {
        std::cout << item.toStdString() << "\t";
    }
    std::cout << "\n";
}

QList<QVariant> MnTable::toVariants(const QStringList &fields) {
    QList<QVariant> params = {};
    for (const QString &fld: fields) {
        //auto f = tableDef.fieldByName(fld);
        auto col =fieldIndex(fld);//  _sql.fields().indexOf(fld);// fTableDef.fieldIndex(fld);
        auto f =field(col);// fTableDef_.fields[col];
        switch (f.field_type) {
            case INTEGER:
                if ((*data())[row][col] == "") {
                    params.append(QVariant());
                } else
                    params.append((*data())[row][col].toInt());
                break;
            case TEXT:
            case VARCHAR:
                params.append((*data())[row][col]);
                break;
            case REAL:
                if ((*data())[row][col] == "") {
                    params.append(QVariant());
                } else
                    params.append((*data())[row][col].toFloat());
                break;
            case BLOB:
                //TODO:BLOB
                break;
            case BOOL:
                if ((*data())[row][col] == "") {
                    params.append(false);
                } else
                    params.append((*data())[row][col].toInt() != 0);
                break;
            case DATETIME:
                if ((*data())[row][col] == "") {
                    params.append(QVariant());
                } else
                    params.append((*data())[row][col]);
                break;
            default:
                throw MNException("field type " + QString::number(f.field_type) + " UNHANDLED");
        }
    }

    return params;
}

QStringList MnTable::rowAt(int row) {
    return (*data())[row];
}

int MnTable::rowNo() {
    return row;
}

bool MnTable::doBeforeSetFieldVal(MnTable *tbl, const QString &oldVal, QString &newVal) {
    bool ret = true;
    for (auto proc: beforeSetFieldValProcs) {
        ret = ret && proc(tbl, oldVal, newVal);
    }
    return ret;
}

bool MnTable::cancel() {
    if (fState == stBrowse) throw MNException("can not cancel when in browse state");
    if (fState == stInsert) {
        data()->removeLast();
        if (!execBeforeScroll()) {
            return false;
        }
        row = (int) data()->count() - 1;
        execAfterScroll();
    } else if (fState == stEdit) {
        if (!fNotEdited) {
            (*data())[row] = fOldVals;
        }
    }
    fOldVals.clear();
    fNotEdited = true;
    fState = stBrowse;
    return true;
}

bool MnTable::remove() {
    if (!execBeforeRemove())
        return false;
    bool ret = true;
    if ((*data())[row][0] != "") {
        ret = conn->exec("DELETE FROM " + tableName() + " WHERE id=" + (*data())[row][0]);//TODO:possible sql injection
    }
    if (ret) {
        data()->removeAt(row);
        execBeforeScroll();
        if (row > 0)
            row--;
        execAfterScroll();
    }
    execAfterRemove();
    return ret;
}

bool MnTable::execBeforeRemove() {
    bool ret = true;
    for (auto proc: beforeRemoveNtfs) {
        ret = ret && proc(this);
    }
    return ret;
}

void MnTable::execAfterRemove() {
    for (auto proc: afterRemoveNtfs) {
        proc(this);
    }
}

bool MnTable::execBeforePost() {
    bool ret = true;
    for (auto proc: beforePostNtfs) {
        ret = ret && proc(this);
    }
    return ret;
}

void MnTable::execAfterPost() {
    for (auto proc: afterPostNtfs) {
        proc(this);
    }
}

bool MnTable::isOpen() const {
    return fActive;
}

MnTableDef MnTable::tableDef() {
    return fTableDef_;
}

bool MnTable::priorFirst() {
    if (row != 0) {
        return goToNoBoundChenck(-1);
    }
    row = -1;
    return true;
}

QList<QStringList> *MnTable::data() const {
    return (QList<QStringList> *)(&_data);
}

void MnTable::setFiltered(bool f) {
    _filtered = f;
}

void MnTable::mntableCopy(MnTable *des,const MnTable &other) {
    if (des != &other) {
        des->_data = *(other.data());
        des->fTableDef_ = other.fTableDef_;
        des->fActive = other.fActive;
        des->row = other.row;
        des->conn = other.conn;
        des->sql_ = other.sql_;
        des->fState =other.fState;
        des->fNotEdited = other.fNotEdited;
        des->fParams = other.fParams;
    }
}

    MnTable &MnTable::operator=(const MnTable &other) {
        if (this->fActive){
            qDebug()<<"can not assign to an open MnTable";
            throw MNException("can not assign to an open MnTable");
        }
        if (this != &other) {
            this->_data.clear();
            mntableCopy(this,other);
        }
        return *this;
    }


MnTable::MnTable(const MnTable &other)
:sql_(other.sql_)
{
    mntableCopy(this,other);
}

bool MnTable::find(const QString &fieldName, const QString &value) {
    int bk = row;
    int rowFound = -1;
    while (row < recordCount()){
        if(fieldByName(fieldName) == value){
            rowFound = row;
            break;
        }
        row ++;
    }
    row = bk;
    if (rowFound !=-1){
        goTo(rowFound);
        return true;
    }
    return false;
}

bool MnTable::findId(int id) {
    return find("id",QString::number(id));
}

bool MnTable::findId(const QString id) {
    return find("id",id);
}

MnView MnTable::filter(const std::function<bool()> &lambda_search) {
    MnTable tbl(*this);
    QList<QStringList> list;
    int bk = row;
    priorFirst();
    while (next()){
        if (lambda_search()){
            list.append((*data())[row]);
        }
    }
    row = bk;
    tbl._data = list;
    return tbl;
}

bool MnTable::isEmpty() {
    return data()->isEmpty();
}

MnFieldDef MnTable::field(int col) {
    return fTableDef_.fields[col];
}

bool MnTable::fieldsContains(const char *string) {
    return sql_.fields().contains(string);
}

void MnTable::fieldsClear() {
    sql_.fieldsClear();
}

void MnTable::fieldAppend(QStringList list) {
    sql_.fieldAppend(list);
}

void MnTable::insertFieldsClear() {
    sql_.insertFieldsClear();
}

QString MnTable::tableName() {
    return sql_.tableName();
}

QStringList MnTable::fields() {
    return sql_.fields();
}

QStringList MnTable::insertFields() {
    return sql_.insertFields();
}


//-------------------
const QString &MnDataSetCol::value() const {
    return _value;
}

void MnDataSetCol::setValue(const QString &value) {
    _value = value;
}

bool MnDataSetCol::isEdited() const {
    return _edited;
}

void MnDataSetCol::setEdited(bool edited) {
    _edited = edited;
}

bool MnDataSetCol::isFiltered() const {
    return _filtered;
}

void MnDataSetCol::setFiltered(bool filtered) {
    _filtered = filtered;
}
