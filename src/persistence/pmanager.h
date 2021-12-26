#ifndef PMANAGER_H
#define PMANAGER_H

#include <sqlite3.h> /* Version: 3.9.2 */
#include <stdio.h>
#include <list>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fstream>
#include <filesystem>
#include "../model/event.h"
#include "../model/category.h"

#define FOLDER_NAME "kalendar"
#define DEFAULT_DATABASE_NAME "default.sql"

using namespace std;
static string db_name = DEFAULT_DATABASE_NAME;

class PManager
{
private:
    sqlite3 *db = NULL;
    string db_path;
    string db_folder;
    string filterSpecialChars(string str);

public:
    PManager(string database = DEFAULT_DATABASE_NAME);
    ~PManager();
    void init_db(string db_name);
    void set_db(string database);
    string get_db_name();
    string get_db_folder();
    vector<string> get_db_list();
    bool add_event (Event *e, Event *child = NULL);
    bool replace_event (Event *old_event, Event *new_event); //return true also if old_event doesn't exist
    bool remove_event(Event *e);
    bool remove_db();
    list<Event*> get_events_of_month(int month, int year);
    list<Event*> get_events(Category *c);
    list<Event*> get_all_events();
    bool add_category (Category *c);
    /* Note: the id will not be changed (to avoid to change the events with a reference to the category */
    bool replace_category(Category *old_category, Category *new_category);
    bool remove_category(Category *c);
    vector<Category*> get_categories();
    Category *get_category(unsigned int id);
    unsigned int get_setting(unsigned int id);
    bool replace_setting(unsigned int setting_id, unsigned int new_value);
    bool remove_past_events(time_t timestamp);
    int save_db(string path);
    int export_db_iCal_format(list<Event *> events, string path);
    int load_db(string path);
    int import_db_iCal_format(string path,Category *category);
};

#endif // PMANAGER_H
