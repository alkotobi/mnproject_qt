#ifndef MNDATASTRUCTURES_H
#define MNDATASTRUCTURES_H

#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class mnfield {
public:
    QString field_name;
    QString field_type;
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
    // bool is_lookup;

    mnfield();
    explicit mnfield(const QString& json_str);
    [[nodiscard]] QString to_json() const;
};

class mntable {
public:
    QString table_name;
    QVector<mnfield> fields;
    QString default_data;
    QString description;
    QString insert_sql;
    int insert_params_count;
    bool is_view;
    QString create_sql;

    mntable();
    explicit mntable(const QString& json_str);
    [[nodiscard]] QString to_json() const;
    mnfield field_by_name(const QString& field_name_to_find);
    int field_index_by_name(const QString& field_name_to_find);
};

class mndatabase {
public:
    QString database_name;
    QVector<mntable> tables;
    double version;
    QString description;
    QString db_path;

    mndatabase();
    explicit mndatabase(const QString& json_str);
    [[nodiscard]] QString to_json() const;
    mntable table_by_name(const QString& table_name_to_find);
};

#endif // MNDATASTRUCTURES_H