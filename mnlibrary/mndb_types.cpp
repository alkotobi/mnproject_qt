#include "mndb_types.h"
#include "mnexception.h"
#include <iostream>
#include "mntable.h"


QString dbTypesToString(DbTypes type) {
    switch (type) {
    case INTEGER:
        return "INTEGER";
    case TEXT:
        return "TEXT";
    case REAL:
        return "REAL";
    case VARCHAR:
        return "VARCHAR";
    case BLOB:
        return "BLOB";
    case BOOL:
        return "BOOL";
    case DATETIME:
        return "DATETIME";
    default:
        throw MNException("dbTypesToString :UNKNOWN TYPE");
    }
}

DbTypes stringToDbType(QString type) {
    if (type == "INTEGER") {
        return INTEGER;
    } else if (type == "TEXT") {
        return TEXT;
    } else if (type == "REAL") {
        return REAL;
    } else if (type == "VARCHAR") {
        return VARCHAR;
    } else if (type == "BLOB") {
        return BLOB;
    } else if (type == "BOOL") {
        return BOOL;
    } else if (type == "DATETIME") {
        return DATETIME;
    } else {
        throw MNException("stringToDbType :UNKNOWN TYPE");
    }
}


MnFieldDef MnFieldDef::fromJson(const QString &json_str)
{
    QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
    if (!doc.isObject()) return {};
    QJsonObject json = doc.object();
    field_name = json["field_name"].toString();
    field_type = stringToDbType(json["field_type"].toString());
    field_length = json["field_length"].toInt();
    is_unique = json["is_unique"].toBool();
    is_not_null = json["is_not_null"].toBool();
    is_indexed = json["is_indexed"].toBool();
    default_value = json["default_value"].toString();
    description = json["description"].toString();
    is_required = json["is_required"].toBool();
    is_visible = json["is_visible"].toBool();
    is_read_only = json["is_read_only"].toBool();
    display_width = json["display_width"].toInt();
    display_label = json["display_label"].toString();
    is_calculated = json["is_calculated"].toBool();
    ind = json["ind"].toInt();
    // is_lookup = json["is_lookup"].toBool();
    return *this;
}

QString MnFieldDef::to_json() const {
    QJsonObject json;
    json["field_name"] = field_name;
    json["field_type"] = dbTypesToString(field_type);
    json["field_length"] = field_length;
    json["is_unique"] = is_unique;
    json["is_not_null"] = is_not_null;
    json["is_indexed"] = is_indexed;
    json["default_value"] = default_value;
    json["description"] = description;
    json["is_required"] = is_required;
    json["is_visible"] = is_visible;
    json["is_read_only"] = is_read_only;
    json["display_width"] = display_width;
    json["display_label"] = display_label;
    json["is_calculated"] = is_calculated;
    json["ind"] = ind;
    // json["is_lookup"] = is_lookup;
    return QJsonDocument(json).toJson();
}


// mntable::mntable()
// {
//     this->table_name ="";
// }



MnTableDef MnTableDef::mntable_from_json(const QString& json_str) {
    QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
    if (!doc.isObject()) return{} ;
    QJsonObject json = doc.object();
    table_name = json["table_name"].toString();
    QJsonArray fields_array = json["fields"].toArray();
    fields.append(mnfieldId);
    for (const QJsonValue& value : fields_array) {
        fields.append(MnFieldDef().fromJson(value.toString()));
    }
    default_data = json["default_data"].toString();
    description = json["description"].toString();
    insert_sql = json["insert_sql"].toString();
    insert_params_count = json["insert_params_count"].toInt();
    is_view = json["is_view"].toBool();
    create_sql = json["create_sql"].toString();
    return *this;
}

QString MnTableDef::toJson() const {
    QJsonObject json;
    json["table_name"] = table_name;
    QJsonArray fields_array;
    for (const MnFieldDef& field : fields) {
        fields_array.append(field.to_json());
    }
    json["fields"] = fields_array;
    json["default_data"] = default_data;
    json["description"] = description;
    json["insert_sql"] = insert_sql;
    json["insert_params_count"] = insert_params_count;
    json["is_view"] = is_view;
    json["create_sql"] = create_sql;
    return QJsonDocument(json).toJson();
}

MnFieldDef MnTableDef::fieldByName(const QString& field_name_to_find) {
    for (const MnFieldDef& field : fields) {
        if (field.field_name == field_name_to_find) {
            return field;
        }
    }
    throw MNException(field_name_to_find+" Not found");
}

int MnTableDef::fieldIndex(const QString& field_name_to_find) {
    for (int i = 0; i < fields.size(); ++i) {
        if (fields[i].field_name == field_name_to_find) {
            return i;
        }
    }
    return -1;
}

QString MnTableDef::selectSql()
{

    QString flds = fields[0].field_name;
    for (int i = 1; i < fields.size(); ++i) {
        flds = flds +"," + fields[i].field_name ;
    }
        QString sql = "SELECT " +flds+" FROM "+table_name;
    return sql;
}

QString MnTableDef::fieldsComaSep() {
    QString flds = fields[0].field_name;
    for (int i = 1; i < fields.size(); ++i) {
        flds = flds +"," + fields[i].field_name ;
    }
    return flds;
}

QStringList MnTableDef::fieldList() {
    QStringList flds = {};
    for (int i = 1; i < fields.size(); ++i) {
         flds .append(fields[i].field_name) ;
    }
    return flds;
}

QString MnTableDef::updateSql() {
    assert(0);
    return QString();
}

QString MnTableDef::inserSql() {
    assert(0);
    return QString();
}

void MnTableDef::print() {

    std::cout << "Table Name: " << table_name.toStdString() << std::endl;
    std::cout << "Description: " << description.toStdString() << std::endl;
    std::cout << "Default Data: " << default_data.toStdString() << std::endl;
    std::cout << "Insert SQL: " << insert_sql.toStdString() << std::endl;
    std::cout << "Insert Params Count: " << insert_params_count << std::endl;
    std::cout << "Is View: " << (is_view? "true" : "false") << std::endl;
    std::cout << "Create SQL: " << create_sql.toStdString() << std::endl;

    std::cout << "Fields:" << std::endl;
    for (const MnFieldDef& field : fields) {
        QString typeStr;
        switch (field.field_type) {
            case INTEGER: typeStr = "INTEGER"; break;
            case TEXT: typeStr = "TEXT"; break;
            case REAL: typeStr = "REAL"; break;
            case VARCHAR: typeStr = "VARCHAR"; break;
            case BLOB: typeStr = "BLOB"; break;
            case BOOL: typeStr = "BOOL"; break;
            case DATETIME: typeStr = "DATETIME"; break;
        }
        std::cout << "  Field Name: " << field.field_name.toStdString() << std::endl;
        std::cout << "    Type: " << typeStr.toStdString() << std::endl;
        std::cout << "    Length: " << field.field_length << std::endl;
        std::cout << "    Unique: " << (field.is_unique? "true" : "false") << std::endl;
        std::cout << "    Not Null: " << (field.is_not_null? "true" : "false") << std::endl;
        std::cout << "    Indexed: " << (field.is_indexed? "true" : "false") << std::endl;
        std::cout << "    Default Value: " << field.default_value.toStdString() << std::endl;
        std::cout << "    Description: " << field.description.toStdString() << std::endl;
        std::cout << "    Required: " << (field.is_required? "true" : "false") << std::endl;
        std::cout << "    Visible: " << (field.is_visible? "true" : "false") << std::endl;
        std::cout << "    Read Only: " << (field.is_read_only? "true" : "false") << std::endl;
        std::cout << "    Display Width: " << field.display_width << std::endl;
        std::cout << "    Display Label: " << field.display_label.toStdString() << std::endl;
        std::cout << "    Calculated: " << (field.is_calculated? "true" : "false") << std::endl;
        std::cout << "    Index: " << field.ind << std::endl;
    }
}

MnDatabaseDef::MnDatabaseDef() {}

MnDatabaseDef::MnDatabaseDef(const QString& json_str) {
    QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
    if (!doc.isObject()) return;
    QJsonObject json = doc.object();
    database_name = json["database_name"].toString();
    QJsonArray tables_array = json["tables"].toArray();
    for (const QJsonValue& value : tables_array) {
        MnTableDef tbl;
        tbl.mntable_from_json(value.toString());
        tables.append(tbl);
    }
    version = json["version"].toDouble();
    description = json["description"].toString();
    db_path = json["db_path"].toString();
}

QString MnDatabaseDef::to_json() const {
    QJsonObject json;
    json["database_name"] = database_name;
    QJsonArray tables_array;
    for (const MnTableDef& table : tables) {
        tables_array.append(table.toJson());
    }
    json["tables"] = tables_array;
    json["version"] = version;
    json["description"] = description;
    json["db_path"] = db_path;
    return QJsonDocument(json).toJson();
}

MnTableDef MnDatabaseDef::table_by_name(const QString& table_name_to_find) {
    for (const MnTableDef& table : tables) {
        if (table.table_name == table_name_to_find) {
            return table;
        }
    }
    throw MNException(table_name_to_find + " not found");
}

MnDatabaseDef::MnDatabaseDef(const QString &dbName, const QVector<MnTableDef> &tbls, double ver)
: database_name(dbName), tables(tbls), version(ver)  {

}

QString mapDbTypeToQString(DbTypes type) {
    switch (type) {
        case INTEGER:
            return "INTEGER";
        case TEXT:
            return "TEXT";
        case REAL:
            return "REAL";
        case VARCHAR:
            return "VARCHAR";
        case BLOB:
            return "BLOB";
        case BOOL:
            return "BOOL";
        case DATETIME:
            return "DATETIME";
        default:
            return "UNKNOWN_TYPE";
    }
}


DbTypes mapQStringToDbType(const QString& str) {
    if (str == "INTEGER") {
        return INTEGER;
    } else if (str == "TEXT") {
        return TEXT;
    } else if (str == "REAL") {
        return REAL;
    } else if (str == "VARCHAR") {
        return VARCHAR;
    } else if (str == "BLOB") {
        return BLOB;
    } else if (str == "BOOL") {
        return BOOL;
    } else if (str == "DATETIME") {
        return DATETIME;
    } else {
        std::cerr << "Unknown database type string: " << str.toStdString() << std::endl;
        return INTEGER; // Default to INTEGER in case of unknown input.
    }
}

MnFieldDef MnFieldDef::init_from_db(MnTable* table) {
    MnFieldDef result;
    result.field_name = table->fieldByName("field_name");
    // Assuming there's a way to convert string to appropriate DbTypes enum value.
    // For simplicity, assuming field_type is a string for now.
    result.field_type = mapQStringToDbType(table->fieldByName("field_type"));
    result.field_length = table->fieldByName("field_length").toInt();
    result.is_unique = table->fieldByName("is_unique").toInt()!= 0;
    result.is_not_null = table->fieldByName("is_not_null").toInt()!= 0;
    result.is_indexed = table->fieldByName("is_indexed").toInt()!= 0;
    result.default_value = table->fieldByName("default_value");
    result.description = table->fieldByName("description");
    result.is_required = table->fieldByName("is_required").toInt()!= 0;
    result.is_visible = table->fieldByName("is_visible").toInt()!= 0;
    result.is_read_only = table->fieldByName("is_read_only").toInt()!= 0;
    result.display_width = table->fieldByName("display_width").toInt();
    result.display_label = table->fieldByName("display_label");
    result.is_calculated = table->fieldByName("is_calculated").toInt()!= 0;
    result.ind = table->fieldByName("ind").toInt();
    return result;
}


MnTableDef MnTableDef::init_from_db(MnTable *tbl_tables, MnTable *tbl_fields) {
    MnTableDef result;
    if (tbl_fields->recordCount() == 0) {
        throw MNException("empty fields");
    }
    if (tbl_tables->recordCount() == 0) {
        throw MNException("empty tables");
    }
    result.table_name = tbl_tables->fieldByName("table_name");
    result.default_data = tbl_tables->fieldByName("default_data");
    result.description = tbl_tables->fieldByName("description");
    result.is_view = tbl_tables->fieldByName("is_view").toInt()!= 0;
    result.create_sql = tbl_tables->fieldByName("create_sql");

    tbl_fields->priorFirst();
    while (tbl_fields->next()) {
        MnFieldDef fld;
        fld.init_from_db(tbl_fields);
        result.fields.push_back(fld);
    }
    result.insert_params_count = result.fields.size();
    return result;
}
