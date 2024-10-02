#include "mntable.h"
#include "mnexception.h"
#include <iostream>
#include "mnview.h"


bool MnTable::exec(const QString &sql, const QList<QVariant> &params) {
    if (conn != nullptr && sql != "") {
        return conn->exec(sql, params);
    } else {
        return false;
    }
}


MnTable::MnTable(mnconnection *conn, MnTableDef table, QObject *parent)
        : QObject(parent), _sql(table.selectSql()) {
    this->conn = conn;
    this->fTableDef = table;

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
    QStringList fld = {};
    QStringList *fldPtr = &fld;
    QString s = sqlText();
    if (_sql.fieldsContains("*")) {
        _sql.fieldsClear();
    } else {
        fldPtr = nullptr;
    }
    bool ret = this->conn->exec(s, params, data(), fldPtr);
    if (fldPtr) {
        if (!fld.contains("id")) {
            fld.insert(0, "id");
        }
        fTableDef = conn->tableDef(_sql.tableName(), fld);
        _sql.fieldAppend(fld);
        _sql.insertFieldsClear();
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
    for (int i = 0; i < _sql.fields().count(); i++) {
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
            int id = this->conn->execInsertSql(_sql.tableName(), _sql.insertFields().join(","),
                                               toVariants(_sql.insertFields()));
            setFieldValue(0, QString::number(id));
            res = id != -1;

        } else {
            //do update
            res = this->conn->execUpdateSql(
                    _sql.tableName(), _sql.insertFields().join(","), "id=" + (*data())[row][0],
                    toVariants(_sql.insertFields()));
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
    if (col < 0 || col > _sql.fields().size()) throw MNException(QString::number(col) + "is not a correct field col");
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


MnTable::MnTable(mnconnection *conn, QString sql, QObject *parent)
        : QObject(parent), _sql(sql) {
    //fTableDef = conn->tableDef(_sql.tableName(), _sql.fields());
    this->conn = conn;
}

QString MnTable::sqlText() {
    return _sql.text();
}

int MnTable::fieldIndex(const QString &fieldName)const {
    return (int) _sql.fields().indexOf(fieldName);
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
    fTableDef.print();

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
        auto col = fTableDef.fieldIndex(fld);
        auto f = fTableDef.fields[col];
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
        ret = conn->exec("DELETE FROM " + _sql.tableName() + " WHERE id=" + (*data())[row][0]);//TODO:possible sql injection
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
    return fTableDef;
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

MnTable &MnTable::operator=(const MnTable &other) {
    if (this != &other) {
        *this = MnTable(other);
    }
    return *this;
}

MnTable::MnTable(const MnTable &other)
:_sql(other._sql)
{
    this->_data=*(other.data());
    this->fTableDef = other.fTableDef;
    this->fActive = other.fActive;
    this->row = other.row;
    this->conn = other.conn;
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
