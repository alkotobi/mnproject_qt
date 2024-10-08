#pragma once
#include <QString>
#include "mnlibrary/mndb_types.h"
//-------------------------------------------------------------------
//--------------------------------groups--------------------------
//-------------------------------------------------------------------
static QString main_groups_table_name="groups";
static QString main_groups_name="name";
static const struct MnFieldDef main_groups_name_def = {
.field_name = "name",
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
static QString main_groups_can_login="can_login";
static const struct MnFieldDef main_groups_can_login_def = {
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
static QString main_groups_can_create_db="can_create_db";
static const struct MnFieldDef main_groups_can_create_db_def = {
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
.ind = 7
};
static QString main_groups_can_edit_db="can_edit_db";
static const struct MnFieldDef main_groups_can_edit_db_def = {
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
.ind = 7
};
static QString main_groups_can_del_db="can_del_db";
static const struct MnFieldDef main_groups_can_del_db_def = {
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
.ind = 7
};
static const MnTableDef main_groups_def ={
        .table_name = "groups",
        .fields={mnfieldId,main_groups_name_def,main_groups_can_login_def,main_groups_can_create_db_def,main_groups_can_edit_db_def,main_groups_can_del_db_def,},
        .default_data = "[",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------users--------------------------
//-------------------------------------------------------------------
static QString main_users_table_name="users";
static QString main_users_name="name";
static const struct MnFieldDef main_users_name_def = {
.field_name = "name",
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
.ind = 7
};
static QString main_users_pass="pass";
static const struct MnFieldDef main_users_pass_def = {
.field_name = "pass",
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
static QString main_users_id_group="id_group";
static const struct MnFieldDef main_users_id_group_def = {
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
static QString main_users_active="active";
static const struct MnFieldDef main_users_active_def = {
.field_name = "active",
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
.ind = 20
};
static const MnTableDef main_users_def ={
        .table_name = "users",
        .fields={mnfieldId,main_users_name_def,main_users_pass_def,main_users_id_group_def,main_users_active_def,},
        .default_data = "[",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------db_info--------------------------
//-------------------------------------------------------------------
static QString main_db_info_table_name="db_info";
static QString main_db_info_date_creation="date_creation";
static const struct MnFieldDef main_db_info_date_creation_def = {
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
static QString main_db_info_id_user_create="id_user_create";
static const struct MnFieldDef main_db_info_id_user_create_def = {
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
static QString main_db_info_name_user_create="name_user_create";
static const struct MnFieldDef main_db_info_name_user_create_def = {
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
static const MnTableDef main_db_info_def ={
        .table_name = "db_info",
        .fields={mnfieldId,main_db_info_date_creation_def,main_db_info_id_user_create_def,main_db_info_name_user_create_def,},
        .default_data = "",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 //-------------------------------------------------------------------
//--------------------------------unite--------------------------
//-------------------------------------------------------------------
static QString main_unite_table_name="unite";
static QString main_unite_name="name";
static const struct MnFieldDef main_unite_name_def = {
.field_name = "name",
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
.display_label = "UNITE",
.is_calculated = false,
.ind = 21
};
static const MnTableDef main_unite_def ={
        .table_name = "unite",
        .fields={mnfieldId,main_unite_name_def,},
        .default_data = "",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 static const MnDatabaseDef main_def ={
   .database_name="main",
   .db_path="",
   .tables={main_groups_def,main_users_def,main_db_info_def,main_unite_def},
   .version=1.0,
   .description=""
};
//-------------------------------------------------------------------
//--------------------------------databases--------------------------
//-------------------------------------------------------------------
static QString options_databases_table_name="databases";
static QString options_databases_db_name="db_name";
static const struct MnFieldDef options_databases_db_name_def = {
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
.ind = 1
};
static QString options_databases_is_server="is_server";
static const struct MnFieldDef options_databases_is_server_def = {
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
.ind = 2
};
static QString options_databases_user_name="user_name";
static const struct MnFieldDef options_databases_user_name_def = {
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
.ind = 5
};
static QString options_databases_password="password";
static const struct MnFieldDef options_databases_password_def = {
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
.ind = 6
};
static QString options_databases_port="port";
static const struct MnFieldDef options_databases_port_def = {
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
.ind = 4
};
static QString options_databases_is_active="is_active";
static const struct MnFieldDef options_databases_is_active_def = {
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
.ind = 7
};
static QString options_databases_server="server";
static const struct MnFieldDef options_databases_server_def = {
.field_name = "server",
.field_type = VARCHAR,
.field_length = 20,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 20,
.display_label = "SERVER",
.is_calculated = false,
.ind = 3
};
static QString options_databases_provider="provider";
static const struct MnFieldDef options_databases_provider_def = {
.field_name = "provider",
.field_type = INTEGER,
.field_length = 10,
.is_unique = false,
.is_not_null = false,
.is_indexed = false,
.default_value = "",
.description = "",
.is_required = false,
.is_visible = true,
.is_read_only = false,
.display_width = 10,
.display_label = "TYPE",
.is_calculated = false,
.ind = 19
};
static const MnTableDef options_databases_def ={
        .table_name = "databases",
        .fields={mnfieldId,options_databases_db_name_def,options_databases_is_server_def,options_databases_user_name_def,options_databases_password_def,options_databases_port_def,options_databases_is_active_def,options_databases_server_def,options_databases_provider_def,},
        .default_data = "",
        .description = "",
        .insert_params_count =0,
        .is_view=false,
        .create_sql = "",
};
 static const MnDatabaseDef options_def ={
   .database_name="options",
   .db_path="",
   .tables={options_databases_def},
   .version=1.0,
   .description=""
};
