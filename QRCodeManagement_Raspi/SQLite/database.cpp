#include "database.h"

Database::Database()
{
    database = QSqlDatabase::addDatabase("QSQLITE");    //  link to a database
    folderPath = qApp->applicationDirPath();    //  get the directory path of the applicaiton
    database.setDatabaseName(QString(folderPath + "/Record.db"));   //  rename the database, if the database does not exists, then create, else if it exists, then use it
//    database.open();
//    QString count_sql_table = "SELECT COUNT(*) FROM RECORD";
//    QSqlQuery sql_query;
//    sql_query.prepare(count_sql_table);
//    sql_query.exec();
//    qDebug() << sql_query.last();

}

void Database::DatabaseOperate()    //  operations to the daatabse
{
    if(database.open())
    {

    }
    database.close();
}

void Database::DatabaseCreate()  //  create the table
{
    while(!database.isOpen())
    {
        database.open();
    }
    if(database.open())
    {
        QSqlQuery sql_query;
        if(!DatabaseTableCheck("RECORD"))
        {
            QString create_sql = "CREATE TABLE RECORD (StdID INT PRIMARY KEY, Name STRING, College STRING, Major STRING, Time STRING)";
            sql_query.prepare(create_sql);
            if(!sql_query.exec())
            {
                qDebug() << "Table Create Failed!";
                qDebug() << sql_query.lastError();
            }
            else
            {
                qDebug() << "Table Created Successfully!";
            }
        }
    }
    database.close();
}

void Database::DatabaseInsert(Record* record)   //  Insert the input record into the table RECORD
{
    while(!database.isOpen())
    {
        database.open();
    }
    if(database.isOpen())
    {
        QSqlQuery sql_query;
        QString insert_sql = "INSERT INTO RECORD VALUES (?,?,?,?,?)";   //  insert values into table
        sql_query.prepare(insert_sql);

        //  create QVariantList values for the input record
        QVariantList StdID;
        QVariantList Name;
        QVariantList College;
        QVariantList Major;
        QVariantList Time;

        StdID.append(record->StdID);
        Name.append(record->Name);
        College.append(record->College);
        Major.append(record->Major);
        Time.append(record->time);

        sql_query.addBindValue(StdID);
        sql_query.addBindValue(Name);
        sql_query.addBindValue(College);
        sql_query.addBindValue(Major);
        sql_query.addBindValue(Time);

        if(!sql_query.execBatch())
        {
            qDebug() << "Insert Values Into Table Failed!";
            qDebug() << sql_query.lastError();
        }
        else
        {
            qDebug() << "Insert Successfully!";
            //---------------------------------------------------
            //  linking to python file should be done here
            //---------------------------------------------------
            //  【NOT FINISH PART】
            //---------------------------------------------------
        }
    }
    database.close();

}

QSqlQueryModel* Database::DatabaseSelect(QString StdID_index) //  select the specific record according to [index]
{
    while(!database.isOpen())
    {
        database.open();
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString("SELECT * FROM RECORD WHERE StdID = '%1'").arg(StdID_index));
    model->setHeaderData(0, Qt::Horizontal, "学号");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "学院");
    model->setHeaderData(3, Qt::Horizontal, "专业");
    model->setHeaderData(4, Qt::Horizontal,"操作时间");
    return model;
}

bool Database::DatabaseTableCheck(QString Name) // Check if the [Name] Table exists in the current database; if exists, return TRUE
{
    QSqlQuery sql_query;
    QString check_sql = "SELECT COUNT(*) FROM SQLITE_MASTER WHERE TYPE='TABLE' AND NAME='%1'";
    sql_query.prepare(check_sql.arg(Name));
    sql_query.exec();
    if(!sql_query.last())
    {
        qDebug() << "There Is No Table Named:" + Name;
        return false;
    }
    else
    {
        qDebug() << "Table Named: " + Name + " Already Exists!";
        return true;
    }
    return false;
}

QSqlQueryModel* Database::DatabaseTableView()   //  show all table record in table
{
    while(!database.isOpen())
    {
        database.open();
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM RECORD");
    model->setHeaderData(0, Qt::Horizontal, "学号");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "学院");
    model->setHeaderData(3, Qt::Horizontal, "专业");
    model->setHeaderData(4, Qt::Horizontal,"操作时间");
    return model;
}


