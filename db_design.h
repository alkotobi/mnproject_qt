#pragma once
#include <QString>
#include "mnlibrary/mndb_types.h"
//-------------------------------------------------------------------
//--------------------------------group--------------------------
//-------------------------------------------------------------------
static QString group_table_name="group";
static QString group_group_name="group_name";
static const struct MnFieldDef group_group_name_def = {
.field_name = "group_name",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = true,
.is_not_null = true,
.is_indexed = true,
.default_value = "",
.description = "",
.is_required = true,
.is_visible = true,
.is_read_only = false,
.display_width = 30,
.display_label = "GROUP",
.is_calculated = false,
.ind = 1
};
static QString group_can_login="can_login";
static const struct MnFieldDef group_can_login_def = {
.field_name = "can_login",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "1",
.description = "",
.is_required = false,
.is_visible = false,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 2
};
static QString group_can_create_db="can_create_db";
static const struct MnFieldDef group_can_create_db_def = {
.field_name = "can_create_db",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "1",
.description = "",
.is_required = false,
.is_visible = false,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 3
};
static QString group_can_edit_db="can_edit_db";
static const struct MnFieldDef group_can_edit_db_def = {
.field_name = "can_edit_db",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "1",
.description = "",
.is_required = false,
.is_visible = false,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 4
};
static QString group_can_del_db="can_del_db";
static const struct MnFieldDef group_can_del_db_def = {
.field_name = "can_del_db",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "1",
.description = "",
.is_required = false,
.is_visible = false,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 5
};
static const MnTableDef group_def ={
        .table_name = "group",
        .fields={group_group_name_def,group_can_login_def,group_can_create_db_def,group_can_edit_db_def,group_can_del_db_def,},
        .default_data = "[",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------user--------------------------
//-------------------------------------------------------------------
static QString user_table_name="user";
static QString user_user_name="user_name";
static const struct MnFieldDef user_user_name_def = {
.field_name = "user_name",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = true,
.is_not_null = true,
.is_indexed = true,
.default_value = "",
.description = "",
.is_required = true,
.is_visible = true,
.is_read_only = false,
.display_width = 30,
.display_label = "UTILISATEUR",
.is_calculated = false,
.ind = 6
};
static QString user_user_pass="user_pass";
static const struct MnFieldDef user_user_pass_def = {
.field_name = "user_pass",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = false,
.is_not_null = true,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = true,
.is_visible = true,
.is_read_only = false,
.display_width = 12,
.display_label = "MOT DE PASS",
.is_calculated = false,
.ind = 7
};
static QString user_id_group="id_group";
static const struct MnFieldDef user_id_group_def = {
.field_name = "id_group",
.field_type = INTEGER,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = false,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 8
};
static const MnTableDef user_def ={
        .table_name = "user",
        .fields={user_user_name_def,user_user_pass_def,user_id_group_def,},
        .default_data = "[",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------databases--------------------------
//-------------------------------------------------------------------
static QString databases_table_name="databases";
static QString databases_db_name="db_name";
static const struct MnFieldDef databases_db_name_def = {
.field_name = "db_name",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = true,
.is_not_null = true,
.is_indexed = true,
.default_value = "",
.description = "p",
.is_required = true,
.is_visible = true,
.is_read_only = false,
.display_width = 20,
.display_label = "DATABASE",
.is_calculated = false,
.ind = 9
};
static QString databases_is_server="is_server";
static const struct MnFieldDef databases_is_server_def = {
.field_name = "is_server",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 10
};
static QString databases_user_name="user_name";
static const struct MnFieldDef databases_user_name_def = {
.field_name = "user_name",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 20,
.display_label = "UTILISATEUR",
.is_calculated = false,
.ind = 11
};
static QString databases_password="password";
static const struct MnFieldDef databases_password_def = {
.field_name = "password",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 12,
.display_label = "MOT DE PASS",
.is_calculated = false,
.ind = 12
};
static QString databases_port="port";
static const struct MnFieldDef databases_port_def = {
.field_name = "port",
.field_type = INTEGER,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 0,
.display_label = "PORTE",
.is_calculated = false,
.ind = 13
};
static QString databases_is_active="is_active";
static const struct MnFieldDef databases_is_active_def = {
.field_name = "is_active",
.field_type = BOOL,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 0,
.display_label = "ACTIVE",
.is_calculated = false,
.ind = 14
};
static const MnTableDef databases_def ={
        .table_name = "databases",
        .fields={databases_db_name_def,databases_is_server_def,databases_user_name_def,databases_password_def,databases_port_def,databases_is_active_def,},
        .default_data = "",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------db_info--------------------------
//-------------------------------------------------------------------
static QString db_info_table_name="db_info";
static QString db_info_date_creation="date_creation";
static const struct MnFieldDef db_info_date_creation_def = {
.field_name = "date_creation",
.field_type = DATETIME,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 15
};
static QString db_info_id_user_create="id_user_create";
static const struct MnFieldDef db_info_id_user_create_def = {
.field_name = "id_user_create",
.field_type = INTEGER,
.field_length = 0,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 0,
.display_label = "",
.is_calculated = false,
.ind = 16
};
static QString db_info_name_user_create="name_user_create";
static const struct MnFieldDef db_info_name_user_create_def = {
.field_name = "name_user_create",
.field_type = VARCHAR,
.field_length = 255,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 15,
.display_label = "",
.is_calculated = false,
.ind = 17
};
static const MnTableDef db_info_def ={
        .table_name = "db_info",
        .fields={db_info_date_creation_def,db_info_id_user_create_def,db_info_name_user_create_def,},
        .default_data = "",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 