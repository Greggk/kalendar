#include "pmanager_test.h"

#include <QDebug>

PManagerTest::PManagerTest()
{
    time_t timestamp = 966038400L; // = 20/12/1999 I think it's better than time(NULL);
    string test("test_string");
    string specialchars("'/--\"#@");
    /* Categories */
    this->valid_default_category = new Category(1, string("Default"), string("#1022A0"));
    this->valid_category = new Category(0, test, test);
    this->valid_category_2 = new Category(100, test, test);
    this->noname_category = new Category(0, string(""), test);
    this->specialchars_category = new Category(0, specialchars, specialchars);
    /* Events */
    this->valid_event = new Event(0, test, test, test, this->valid_default_category, timestamp, timestamp + 100);
    this->valid_event_2 = new Event(100, test, test, test, this->valid_default_category, timestamp - 500, timestamp + 1000000); //starts from current month, ends the next month
    this->valid_event_3 = new Event(80, test, test, test, this->valid_category_2, timestamp - 500, timestamp + 1000000);
    /* Invalid Events */
    this->event_with_null_category = new Event(0, test, test, test, NULL, timestamp, timestamp);
    this->event_with_invalid_category = new Event(0, test, test, test, new Category(99, test, test), timestamp, timestamp);
    this->noname_event = new Event(1, string(""), test, test, new Category(1, test, test), timestamp, timestamp + 100);
    this->invalid_time_event = new Event(1, test, test, test,  new Category(1, test, test), timestamp, timestamp - 100);
    this->specialchars_event = new Event(1, specialchars, specialchars, specialchars, new Category(1, specialchars, specialchars), timestamp, timestamp + 100);
}

PManagerTest::~PManagerTest() {
    delete this->valid_event;
    delete this->valid_event_2;
    delete this->valid_event_3;
    delete this->noname_event;
    delete this->invalid_time_event;
    delete this->noname_category;
    delete this->valid_category;
    delete this->specialchars_category;
    delete this->valid_category_2;
    delete this->valid_default_category;
    delete this->specialchars_event;
    delete this->event_with_invalid_category;
    delete this->event_with_null_category;
}

void PManagerTest::test_all() {
    test_get_db_name();
    test_remove_db();
    test_init_db();
    test_get_db_list();
    test_add_event();
    test_get_events_of_month();
    test_remove_event();
    test_add_category();
    test_get_categories();
    test_remove_category();
    test_get_category();
    test_edit_event();
    test_get_all_events();
    test_get_events();
    test_remove_past_events();
    test_edit_category();
    test_set_db();
    test_load_db();
    test_save_db();
    test_import_db_iCal_format();
    test_export_db_iCal_format();
}

void PManagerTest::test_get_db_name() {
    Test::print("test_get_db_name ");
    PManager pm;
    bool ret = (pm.get_db_name() == DEFAULT_DATABASE_NAME);
    PManager pm2("test.sql");
    ASSERT (ret && (pm2.get_db_name() == "test.sql"))
    pm.remove_db();
    pm2.remove_db();
}

void PManagerTest::test_remove_db() {
    Test::print("test_remove_db ");
    PManager pm;
    bool ret = pm.remove_db();
    if (pm.get_db_name() != DEFAULT_DATABASE_NAME) {
        ret = ret && !std::filesystem::exists(pm.get_db_folder() + pm.get_db_name());
    } else {
        ret = ret && std::filesystem::exists(pm.get_db_folder() + pm.get_db_name());
    }
    ASSERT (ret)
}

void PManagerTest::test_init_db() {
    Test::print("test_init_db ");
    PManager pm;
    pm.init_db("test.sql");
    ASSERT (std::filesystem::exists(pm.get_db_folder() + "test.sql"))
    pm.remove_db();
}

void PManagerTest::test_get_db_list() {
    Test::print("test_get_db_list ");
    string names[] = {"test1.sql", "test2.sql"};
    PManager pm;
    pm.remove_db();
    pm.init_db(names[0]);
    bool ret = (std::filesystem::exists(pm.get_db_folder() + names[0]));
    pm.remove_db();
    pm.init_db(names[1]);
    ASSERT (ret && std::filesystem::exists(pm.get_db_folder() + names[1]))
    pm.remove_db();
}

void PManagerTest::test_add_event() {
    Test::print("test_add_event ");
    PManager pm;
    ASSERT ((!(pm.add_event(this->noname_event))) &&
           (!(pm.add_event(this->invalid_time_event))) &&
           (!(pm.add_event(this->event_with_invalid_category))) &&
           (!(pm.add_event(this->event_with_null_category))) &&
           (pm.add_event(this->specialchars_event)) &&
           (pm.add_event(this->valid_event)))
    pm.remove_db();
}

void PManagerTest::test_get_events_of_month() {
    Test::print("test_get_events_of_month ");
    bool ret = false, ret2 = false;
    PManager pm;
    pm.add_event(this->valid_event);
    time_t timestamp = this->valid_event->getStart();
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900); // tm_mon is from 0 to 11, we need to have 1 - 12
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret = this->valid_event->equals(**it); // *it has type Event*
        delete *it;
    }
    pm.add_event(this->valid_event_2);
    timestamp = this->valid_event_2->getEnd();
    current_time = localtime(&timestamp);
    events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900);
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret2 = this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    }
    ASSERT (ret && ret2)
    pm.remove_db();
}

void PManagerTest::test_remove_event() {
    Test::print("test_remove_event ");
    bool ret;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    ret = pm.remove_event(this->valid_event);
    time_t timestamp = this->valid_event->getStart();
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900);
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = ret && this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_db();
}

void PManagerTest::test_add_category() {
    Test::print("test_add_category ");
    PManager pm;
    ASSERT ((!(pm.add_category(this->noname_category))) && (pm.add_category(this->valid_category)))
    pm.remove_db();
}

void PManagerTest::test_get_categories() {
    Test::print("test_get_categories ");
    bool ret = false;
    PManager pm;
    pm.add_category(this->valid_category);
    vector<Category*> categories = pm.get_categories();
    if (!(categories.empty())) {
        vector<Category*>::iterator it = categories.begin();
        ret = (this->valid_category->equals(**it) || this->valid_default_category->equals(**it)); // *it has type Category*
        delete *it;
        it++;
        ret = ret && (this->valid_category->equals(**it) || this->valid_default_category->equals(**it)); // *it has type Category*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_db();
}

void PManagerTest::test_remove_category() {
    Test::print("test_remove_category ");
    bool ret;
    PManager pm;
    pm.add_category(this->valid_category);
    pm.add_category(this->valid_category_2);
    ret = pm.remove_category(this->valid_category);
    ret = ret && pm.remove_category(this->valid_default_category); //Delete the default category
    vector<Category*> categories = pm.get_categories();
    if (categories.size() == 1) {
        vector<Category*>::iterator it = categories.begin();
        ret = ret && this->valid_category_2->equals(**it); // *it has type Category*
        delete *it;
    } else ret = false;
    ret = ret && pm.remove_category(this->valid_category_2);
    ret = ret && (pm.get_categories().size() == 0);
    pm.add_category(this->valid_default_category);
    pm.add_event(this->valid_event);
    ret = ret && (!pm.remove_category(this->valid_default_category)); //try to delete the default category, but it's referenced by valid_event, so the function should fails
    categories = pm.get_categories();
    if (categories.size() == 1) {
        vector<Category*>::iterator it = categories.begin();
        ret = ret && this->valid_default_category->equals(**it); // *it has type Category*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_db();
}

void PManagerTest::test_get_category() {
    Test::print("test_get_category ");
    PManager pm;
    pm.add_category(this->valid_category);
    ASSERT ((pm.get_category(this->valid_category->getId()))->equals(*this->valid_category))
    pm.remove_db();
}

void PManagerTest::test_edit_event() {
    Test::print("test_edit_event ");
    PManager pm;
    pm.add_event(this->valid_event);
    ASSERT (pm.replace_event(this->valid_event, this->valid_event_2) &&
            (!pm.replace_event(this->valid_event_2, this->noname_event)))
    pm.remove_db();
}

void PManagerTest::test_get_all_events() {
    Test::print("test_get_all_events ");
    bool ret = false, ret2 = false;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    list<Event*> events = pm.get_all_events();
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret = this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
        it++;
        ret2 = this->valid_event->equals(**it);
        delete *it;
    }
    ASSERT (ret && ret2)
    pm.remove_db();
}

void PManagerTest::test_get_events() {
    Test::print("test_get_events ");
    bool ret = false;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_category(this->valid_category_2);
    pm.add_event(this->valid_event_3);
    list<Event*> events_cat1 = pm.get_events(this->valid_default_category);
    ret = (events_cat1.size() == 1);
    list<Event*>::iterator it1 = events_cat1.begin();
    ret = ret && this->valid_event->equals(**it1);
    delete *it1;
    list<Event*> events_cat2 = pm.get_events(this->valid_category_2);
    ret = ret && (events_cat2.size() == 1);
    list<Event*>::iterator it2 = events_cat2.begin();
    ret = ret && this->valid_event_3->equals(**it2);
    delete *it2;
    list<Event*> events = pm.get_events(NULL);
    ret = ret && (events.size() == 2);
    ASSERT (ret)
    pm.remove_db();
}

void PManagerTest::test_remove_past_events() {
    Test::print("test_remove_past_events ");
    bool ret;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    ret = pm.remove_past_events(this->valid_event_2->getEnd()-1);
    list<Event*> events = pm.get_all_events();
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = ret && this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_db();
}

void PManagerTest::test_edit_category() {
    Test::print("test_edit_category ");
    PManager pm;
    pm.add_category(this->valid_category);
    ASSERT (!pm.replace_category(this->valid_category, this->noname_category) &&
            (pm.replace_category(this->valid_category, this->valid_category_2)))
    pm.remove_db();
}

void PManagerTest::test_set_db() {
    Test::print("test_set_db ");
    PManager pm;
    pm.init_db("test.sql");
    pm.set_db(DEFAULT_DATABASE_NAME);
    bool ret (pm.get_db_name() == DEFAULT_DATABASE_NAME);
    pm.add_category(this->valid_category);
    pm.add_event(this->valid_event);
    pm.set_db("test.sql");
    ret = ret && (pm.get_db_name() == "test.sql");
    pm.add_category(this->valid_category_2);
    pm.add_event(this->valid_event_2);
    pm.add_event(this->valid_event_3);
    pm.set_db(DEFAULT_DATABASE_NAME);
    ret = ret && (pm.get_db_name() == DEFAULT_DATABASE_NAME);
    ret = ret && ((pm.get_all_events().size() == 1) &&
                    (pm.get_all_events().front()->equals(*this->valid_event)));
    pm.remove_db();
    pm.set_db("test.sql");
    ret = ret && (pm.get_db_name() == "test.sql");
    ASSERT (ret && (pm.get_all_events().size() == 2))
    pm.remove_db();
}

void PManagerTest::test_load_db() {
    Test::print("test_load_db ");
    bool ret;
    PManager pm;
    ofstream file;
    file.open("testdb.kal");
    file << "INSERT INTO Categories VALUES(" << this->valid_category->getId() << ",'" << this->valid_category->getName() << "','" << this->valid_category->getColor() << "');" << endl;
    file << "INSERT INTO Events VALUES(" << this->valid_event->getId() << ",'" << this->valid_event->getName() << "','" << this->valid_event->getDescription() << "','" << this->valid_event->getPlace() << "'," << this->valid_event->getCategory()->getId() << "," << this->valid_event->getStart() << "," << this->valid_event->getEnd() << ", NULL);" << endl;
    file.close();
    ret = pm.load_db("");
    ret = !ret && pm.load_db("notexist");
    ret = !ret && pm.load_db("testdb.kal");
    Category *category = pm.get_category(this->valid_category->getId());
    ret = ret && category->equals(*this->valid_category);
    delete category;
    list<Event*> events = pm.get_all_events();
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = ret && this->valid_event->equals(**it); // *it has type Event*
        delete *it;
    } else ret = false;
    pm.remove_db();
    remove("testdb.kal");
    ASSERT (ret)
}

void PManagerTest::test_save_db() {
    Test::print("test_save_db ");
    bool ret;
    PManager pm;
    ifstream file;
    string line;
    char insert_event[1024];
    char insert_category[1024];
    snprintf(insert_event, 1024, "INSERT INTO Events VALUES(%u, '%s', '%s', '%s', %u, %ld, %ld);", this->valid_event->getId(), this->valid_event->getName().c_str(), this->valid_event->getDescription().c_str(), this->valid_event->getPlace().c_str(), this->valid_event->getCategory()->getId(), this->valid_event->getStart(), this->valid_event->getEnd());
    snprintf(insert_category, 1024, "INSERT INTO Categories VALUES(%u, '%s', '%s');", this->valid_default_category->getId(), this->valid_default_category->getName().c_str(), this->valid_default_category->getColor().c_str());
    pm.add_event(this->valid_event);
    ret = pm.save_db("testdb");
    file.open("testdb.kal");
    getline (file,line);
    ret = ret && ((line == insert_category) || (line == insert_event));
    getline (file,line);
    ret = ret && ((line == insert_event) || (line == insert_category));
    getline (file,line);
    ret = ret && (line == "");
    file.close();
    pm.remove_db();
    remove("testdb.kal");
    ASSERT (ret)
}

void PManagerTest::test_import_db_iCal_format() {
    Test::print("test_import_db_iCal_format ");
    PManager pm;
    ofstream file;
    bool ret;
    ret = pm.import_db_iCal_format("",this->valid_default_category);
    ret = !ret && !pm.import_db_iCal_format("notexists",this->valid_default_category);
    file.open("temp.ics");
    file << "BEGIN:VEVENT" << endl << "UID:0" << endl << "DTSTART;VALUE=DATE:20161231" << endl << "DTEND;VALUE=DATE:20170101" << endl << "SUMMARY:test" << endl << "DESCRIPTION:multi\nline\ndescription" << endl << "END:VEVENT" << endl;
    file << "BEGIN:VEVENT" << endl << "UID:1" << endl << "DTSTART;VALUE=DATE:20130512" << endl << "DTEND;VALUE=DATE:20130513" << endl << "SUMMARY:test2" << endl << "END:VEVENT" << endl;
    file.close();
    ret = ret && pm.import_db_iCal_format("temp.ics",this->valid_default_category);
    list<Event*> events = pm.get_all_events();
    if (events.size() == 2) {
        list<Event*>::iterator it = events.begin();
        ret = ret && ((**it).getName() == "test2");
        ret = ret && ((**it).getCategory()->getId() == this->valid_default_category->getId());
        //TODO: checks if the following test is correct
        ret = ret && ((**it).getStart() < (**it).getEnd());
        delete *it;
    } else ret = false;
    pm.remove_db();
    remove("temp.ics");
    ASSERT (ret)
}

void PManagerTest::test_export_db_iCal_format() {
    Test::print("test_export_db_iCal_format ");
    string line;
    PManager pm;
    pm.add_event(this->valid_event);
    ifstream file;
    bool ret;
    ret = pm.export_db_iCal_format(pm.get_all_events(),"");
    ret = !ret && pm.export_db_iCal_format(pm.get_all_events(),"temp.ics");
    file.open("temp.ics");
    int linenumber = 0;
    while ( getline (file,line,'\n') ) {

        if (linenumber == 0) {
            ret = ret && (line == "BEGIN:VCALENDAR");
            linenumber++;
            continue;
        }
        if (linenumber == 3) {
            char buff[9];
            time_t start = this->valid_event->getStart();
            strftime(buff, sizeof(buff),"%Y%m%d",localtime((const time_t*)&start));
            ret = ret && (line == string("DTSTART;VALUE=DATE:") + string(buff));
            linenumber++;
            continue;
        }
        if (linenumber == 5) {
            ret = ret && (line == string("UID:") + to_string(this->valid_event->getId()));
            linenumber++;
            continue;
        }
        if (linenumber == 8) {
            ret = ret && (line == string("SUMMARY:") + this->valid_event->getName());
            linenumber++;
            continue;
        }
        linenumber++;
    }
    file.close();
    pm.remove_db();
    remove("temp.ics");
    ASSERT (ret)
}
