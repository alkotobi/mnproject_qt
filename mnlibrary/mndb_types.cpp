#include "mndb_types.h"
#include "mnexception.h"


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


MnField MnField::fromJson(const QString &json_str)
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

QString MnField::to_json() const {
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



mntable mntable::mntable_from_json(const QString& json_str) {
    QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
    if (!doc.isObject()) return{} ;
    QJsonObject json = doc.object();
    table_name = json["table_name"].toString();
    QJsonArray fields_array = json["fields"].toArray();
    for (const QJsonValue& value : fields_array) {
        fields.append(MnField().fromJson(value.toString()));
    }
    default_data = json["default_data"].toString();
    description = json["description"].toString();
    insert_sql = json["insert_sql"].toString();
    insert_params_count = json["insert_params_count"].toInt();
    is_view = json["is_view"].toBool();
    create_sql = json["create_sql"].toString();
    return *this;
}

QString mntable::to_json() const {
    QJsonObject json;
    json["table_name"] = table_name;
    QJsonArray fields_array;
    for (const MnField& field : fields) {
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

MnField mntable::field_by_name(const QString& field_name_to_find) {
    for (const MnField& field : fields) {
        if (field.field_name == field_name_to_find) {
            return field;
        }
    }
    throw MNException(field_name_to_find+" Not found");
}

int mntable::field_index_by_name(const QString& field_name_to_find) {
    for (int i = 0; i < fields.size(); ++i) {
        if (fields[i].field_name == field_name_to_find) {
            return i;
        }
    }
    return -1;
}

QString mntable::select_sql()
{

    QString flds = fields[0].field_name;
    for (int i = 1; i < fields.size(); ++i) {
        flds = flds +"," + fields[i].field_name ;
    }
        QString sql = "SELECT " +flds+" FORM "+table_name;
    return sql;
}

mndatabase::mndatabase() {}

mndatabase::mndatabase(const QString& json_str) {
    QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
    if (!doc.isObject()) return;
    QJsonObject json = doc.object();
    database_name = json["database_name"].toString();
    QJsonArray tables_array = json["tables"].toArray();
    for (const QJsonValue& value : tables_array) {
        mntable tbl;
        tbl.mntable_from_json(value.toString());
        tables.append(tbl);
    }
    version = json["version"].toDouble();
    description = json["description"].toString();
    db_path = json["db_path"].toString();
}

QString mndatabase::to_json() const {
    QJsonObject json;
    json["database_name"] = database_name;
    QJsonArray tables_array;
    for (const mntable& table : tables) {
        tables_array.append(table.to_json());
    }
    json["tables"] = tables_array;
    json["version"] = version;
    json["description"] = description;
    json["db_path"] = db_path;
    return QJsonDocument(json).toJson();
}

mntable mndatabase::table_by_name(const QString& table_name_to_find) {
    for (const mntable& table : tables) {
        if (table.table_name == table_name_to_find) {
            return table;
        }
    }
    throw MNException(table_name_to_find + " not found");
}


// int main(){
//     printf("hello world\n");
// };
