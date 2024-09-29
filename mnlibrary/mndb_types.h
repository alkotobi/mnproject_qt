#pragma once

#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MnTable;

typedef enum{INTEGER,TEXT,REAL,VARCHAR,BLOB,BOOL,DATETIME} DbTypes;

struct MnFieldDef {
public:
    MnFieldDef fromJson(const QString& json_str);
    QString field_name;
    DbTypes field_type;
    int field_length=0;
    bool is_unique= false;
    bool is_not_null= false;
    bool is_indexed= false;
    QString default_value="";
    QString description="";
    bool is_required= false;
    bool is_visible= true;
    bool is_read_only= false;
    int display_width=30;
    QString display_label="";
    bool is_calculated= false;
    int ind=-1;
    QString to_json() const;
    MnFieldDef init_from_db(MnTable *table);
};

struct MnTableDef {
public:
    QString table_name;
    MnTableDef mntable_from_json(const QString& json_str);
    QVector<MnFieldDef> fields;
    QString default_data="";
    QString description="";
    QString insert_sql="";
    int insert_params_count=0;
    bool is_view=false;
    QString create_sql="";
    QString toJson() const;
    MnFieldDef fieldByName(const QString& field_name_to_find);
    int fieldIndex(const QString& field_name_to_find);
    QStringList fieldList();
    QString fieldsComaSep();
    QString selectSql();
    QString inserSql();
    QString updateSql();
    void print();
    MnTableDef init_from_db(MnTable *tbl_tables, MnTable *tbl_fields);
};

const struct MnFieldDef mnfieldId = {
        .field_name = "id",
        .field_type = INTEGER,
        .field_length = 0,
        .is_unique = true,
        .is_not_null = true,
        .is_indexed = false,
        .default_value = "",
        .description = "",
        .is_required = true,
        .is_visible = false,
        .is_read_only = false,
        .display_width = 20,
        .display_label = "",
        .is_calculated = false,
        .ind = 0
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
    MnDatabaseDef(const QString& dbName, const QVector<MnTableDef>& tbls, double ver);
    [[nodiscard]] QString to_json() const;
    MnTableDef table_by_name(const QString& table_name_to_find);
};


