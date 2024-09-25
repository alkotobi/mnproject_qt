#pragma once

#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

typedef enum{INTEGER,TEXT,REAL,VARCHAR,BLOB,BOOL,DATETIME} DbTypes;

struct MnFieldDef {
public:
    MnFieldDef fromJson(const QString& json_str);
    QString field_name;
    DbTypes field_type;
    int field_length;
    bool is_unique;
    bool is_not_null;
    bool is_indexed;
    QString default_value;
    QString description;
    bool is_required;
    bool is_visible;
    bool is_read_only;
    int display_width;
    QString display_label;
    bool is_calculated;
    int ind;
    QString to_json() const;
};

class MnTableDef {
public:
    QString table_name;
    MnTableDef mntable_from_json(const QString& json_str);
    QVector<MnFieldDef> fields;
    QString default_data;
    QString description;
    QString insert_sql;
    int insert_params_count;
    bool is_view;
    QString create_sql;
    QString toJson() const;
    MnFieldDef fieldByName(const QString& field_name_to_find);
    int fieldIndex(const QString& field_name_to_find);
    QStringList fieldList();
    QString fieldsComaSep();
    QString selectSql();
    QString inserSql();
    QString updateSql();
};

class MnDatabaseDef {
public:
    QString database_name;
    QVector<MnTableDef> tables;
    double version;
    QString description;
    QString db_path;
    MnDatabaseDef();
    explicit MnDatabaseDef(const QString& json_str);
    [[nodiscard]] QString to_json() const;
    MnTableDef table_by_name(const QString& table_name_to_find);
};


