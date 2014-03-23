#include "WeightCurve.h"

#include <assert.h>

#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlField>
#include <QDateTime>
#include <QVariant>

#if defined(QT_DEBUG)
    QString  WeightCurve::dbFileName = "weight_debug.db";
#else
    QString  WeightCurve::dbFileName = "weight.db";
#endif

WeightCurve* WeightCurve::instance = nullptr;

WeightCurve* WeightCurve::singleton()
{
    if (!instance)
        instance = new WeightCurve();
    return instance;
}

void WeightCurve::destroy()
{
    delete instance;
    instance = nullptr;
}

void    WeightCurve::initialize()
{
    QString backupPath;
    QDir    dir;

    assert(!mDB.isOpen());

    backupPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    dir.mkpath(backupPath);
    mDBFilePath = backupPath + "/" + dbFileName;
    qDebug("DB file path: %s\n", mDBFilePath.toUtf8().data());

    // Find QSLite driver
    mDB = QSqlDatabase::addDatabase("QSQLITE");
    mDB.setDatabaseName(mDBFilePath);

    if (!mDB.open())
        qFatal("Can't open database: %s\n", mDB.lastError().text().toUtf8().data());

    if (mDB.tables().empty())
    {
        QSqlQuery query;

        query.exec("CREATE TABLE Weight "
                   "(id        INTEGER          PRIMARY KEY AUTOINCREMENT NOT NULL, "
                   "date       DATE, "
                   "morning    DECIMAL, "
                   "noon       DECIMAL, "
                   "evening    DECIMAL, "
                   "comment    TEXT, "
                   "recordDate DATETIME         DEFAULT CURRENT_TIMESTAMP)");
        if (query.lastError().isValid())
            qFatal("Can't create tables: %s\n", query.lastError().text().toUtf8().data());
    }
    else
    {
        QSqlQuery query;

        query.exec("SELECT date FROM Weight");
        for (int i = 0; query.next(); i++)
        {
            QString date;
            QString updatedDate;

            date = query.value(query.record().indexOf("date")).toString();

            updatedDate = date;
            updatedDate.replace("/", "-");
            QSqlQuery updateQuery;
            updateQuery.exec("UPDATE Weight SET date='" + updatedDate + "' WHERE date='" + date + "'");
        }
    }
}

bool WeightCurve::importFromCSVFile(const QUrl& sourceFilePath)
{
    QFile   cvsFile;

    cvsFile.setFileName(sourceFilePath.toLocalFile());
    if (!cvsFile.open(QIODevice::ReadOnly))
    {
        qFatal("Can't open file: %s\n", sourceFilePath.toLocalFile().toUtf8().data());
        return false;
    }

    QStringList dates;
    QStringList mornings;
    QStringList noons;
    QStringList evenings;
    QStringList comments;

    dates = QString(cvsFile.readLine()).split(";");
    mornings = QString(cvsFile.readLine().replace(",", ".")).split(";");
    noons = QString(cvsFile.readLine().replace(",", ".")).split(";");
    evenings = QString(cvsFile.readLine().replace(",", ".")).split(";");
    comments = QString(cvsFile.readLine()).split(";");

    QSqlQuery query;

    for (int i = 0; i < dates.size(); i++)
    {
        QString date;
        QRegExp rx("([0-9]{2}/[0-9]{2}/[0-9]{4})");
        rx.indexIn(dates[i]);

        date = rx.cap(1);

        // TODO improve performances with a batch
        if (!date.isEmpty())
        {
            query.exec(QString("INSERT INTO Weight (date, morning, noon, evening, comment) VALUES ('%1', '%2', '%3', '%4', '%5')")
                       .arg(date).arg(mornings[i]).arg(noons[i]).arg(evenings[i]).arg(comments[i]));
        }
    }

    if (query.lastError().isValid())
        qFatal("Can't create tables: %s\n", query.lastError().text().toUtf8().data());

    emit morningWeightChanged(morningWeight());
    emit noonWeightChanged(morningWeight());
    emit eveningWeightChanged(morningWeight());

    return true;
}

bool WeightCurve::exportToCSVFile(const QUrl& destinationFilePath)
{
    (void)destinationFilePath;
    return false;
}

void WeightCurve::setMorningWeight(float weight)
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();

    query.exec(QString("SELECT date FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        query.exec(QString("UPDATE Weight SET morning='%1' WHERE date='%2'")
                   .arg(weight).arg(date.toString("dd-MM-yyyy")));
    }
    else
    {
        query.exec(QString("INSERT INTO Weight (date, morning) VALUES ('%1', '%2')")
                   .arg(date.toString("dd-MM-yyyy")).arg(weight));
    }

    emit morningWeightChanged(weight);
    updateCurve("morning", mMorningCurve);
    emit morningCurveChanged(morningCurve());
}

float WeightCurve::morningWeight() const
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();
    bool        ok;
    float       result = std::numeric_limits<float>::quiet_NaN();

    query.exec(QString("SELECT morning FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        float tmp = query.value(query.record().indexOf("morning")).toString().toFloat(&ok);
        if (ok)
            result = tmp;
    }
    return result;
}

void WeightCurve::setNoonWeight(float weight)
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();

    query.exec(QString("SELECT date FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        query.exec(QString("UPDATE Weight SET noon='%1' WHERE date='%2'")
                   .arg(weight).arg(date.toString("dd-MM-yyyy")));
    }
    else
    {
        query.exec(QString("INSERT INTO Weight (date, noon) VALUES ('%1', '%2')")
                   .arg(date.toString("dd-MM-yyyy")).arg(weight));
    }

    emit noonWeightChanged(weight);
    updateCurve("noon", mNoonCurve);
    emit noonCurveChanged(noonCurve());
}

float WeightCurve::noonWeight() const
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();
    bool        ok;
    float       result = std::numeric_limits<float>::quiet_NaN();

    query.exec(QString("SELECT noon FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        float tmp = query.value(query.record().indexOf("noon")).toString().toFloat(&ok);
        if (ok)
            result = tmp;
    }
    return result;
}

void WeightCurve::setEveningWeight(float weight)
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();

    query.exec(QString("SELECT date FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        query.exec(QString("UPDATE Weight SET evening='%1' WHERE date='%2'")
                   .arg(weight).arg(date.toString("dd-MM-yyyy")));
    }
    else
    {
        query.exec(QString("INSERT INTO Weight (date, evening) VALUES ('%1', '%2')")
                   .arg(date.toString("dd-MM-yyyy")).arg(weight));
    }

    emit eveningWeightChanged(weight);
    updateCurve("evening", mEveningCurve);
    emit eveningCurveChanged(eveningCurve());
}

float WeightCurve::eveningWeight() const
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();
    bool        ok;
    float       result = std::numeric_limits<float>::quiet_NaN();

    query.exec(QString("SELECT evening FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        float tmp = query.value(query.record().indexOf("evening")).toString().toFloat(&ok);
        if (ok)
            result = tmp;
    }
    return result;
}

void WeightCurve::setComment(const QString& comment)
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();
    QSqlField   commentField("comment", QVariant::String);
    QString     text;

    commentField.setValue(comment);
    text = mDB.driver()->formatValue(commentField); // Necessary to avoid issue with characters like quotes,...

    query.exec(QString("SELECT comment FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        query.exec(QString("UPDATE Weight SET comment=%1 WHERE date='%2'")
                   .arg(text).arg(date.toString("dd-MM-yyyy")));
    }
    else
    {
        query.exec(QString("INSERT INTO Weight (date, comment) VALUES ('%1', %2)")
                   .arg(date.toString("dd-MM-yyyy")).arg(text));
    }

    emit commentChanged(comment);
    updateCurves();
}

QString WeightCurve::comment() const
{
    QSqlQuery   query;
    QDate       date = QDate::currentDate();
    QString     result;

    query.exec(QString("SELECT comment FROM Weight WHERE date='%1'")
               .arg(date.toString("dd-MM-yyyy")));
    if (query.next())
    {
        result = query.value(query.record().indexOf("comment")).toString();
    }
    return result;
}

Curve *WeightCurve::morningCurve()
{
    return &mMorningCurve;
}

Curve* WeightCurve::noonCurve()
{
    return &mNoonCurve;
}

Curve* WeightCurve::eveningCurve()
{
    return &mEveningCurve;
}

void WeightCurve::setStartDate(const QDate& date)
{
    mStartDate = date;
    emit startDateChanged(mStartDate);
    updateCurves();
}

QDate WeightCurve::startDate() const
{
    return mStartDate;
}

void WeightCurve::setEndDate(const QDate& date)
{
    mEndDate = date;
    emit endDateChanged(mEndDate);
    updateCurves();
}

QDate WeightCurve::endDate() const
{
    return mEndDate;
}

void WeightCurve::onTimerUpdated()
{
    static QDate    oldDate = QDate::currentDate();
    QDate           currentDate = QDate::currentDate();

    if (currentDate != oldDate)
    {
        oldDate = currentDate;
        emit dateChanged();
    }
}

void WeightCurve::onDateChanged()
{
    emit morningWeightChanged(morningWeight());
    emit noonWeightChanged(noonWeight());
    emit eveningWeightChanged(eveningWeight());
}

WeightCurve::WeightCurve()
{
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimerUpdated()));
    QObject::connect(this, SIGNAL(dateChanged()), this, SLOT(onDateChanged()));
    mTimer.setSingleShot(false);
    mTimer.setInterval(0);
    mTimer.start();
}

WeightCurve::~WeightCurve()
{
    mMorningCurve.setColor(QColor("#4F81BD"));
    mNoonCurve.setColor(QColor("#268E2A"));
    mEveningCurve.setColor(QColor("#FF9523"));
}

void WeightCurve::updateCurves()
{
    updateCurve("morning", mMorningCurve);
    mMorningCurve.setColor(QColor("#4F81BD"));
    emit morningCurveChanged(&mMorningCurve);

    updateCurve("noon", mNoonCurve);
    mNoonCurve.setColor(QColor("#268E2A"));
    emit noonCurveChanged(&mNoonCurve);

    updateCurve("evening", mEveningCurve);
    mEveningCurve.setColor(QColor("#FF9523"));
    emit eveningCurveChanged(&mEveningCurve);
}

void WeightCurve::updateCurve(const QString &columnName, Curve &curve)
{
    QString         queryString;
    QSqlQuery       query;
    QVariantList    points;
    QStringList     comments;
    float           min = std::numeric_limits<float>::max();
    float           max = std::numeric_limits<float>::min();

    if (!mStartDate.isValid() || !mEndDate.isValid())
        return;

    queryString = "SELECT date, " + columnName + ", comment FROM Weight";

    // Dates range is made manually cause of an issue with this request
/*    queryString = "SELECT date, " + columnName + ", comment FROM Weight"
                       " date BETWEEN '" + mStartDate.toString("yyyy-MM-dd") + "'"
                       " AND '" + mEndDate.toString("yyyy-MM-dd") + "'";*/
    query.exec(queryString);
    if (query.lastError().isValid())
        qWarning("Failed to retrieve values : %s\n", query.lastError().text().toUtf8().data());

    for (int i = 0; query.next();)
    {
        float   value;
        QPointF point;
        QDate   date;
        bool    ok;

        date = QDate::fromString(query.value(query.record().indexOf("date")).toString(), "dd-MM-yyyy");

        if (!(date > mStartDate && date <= mEndDate))
            continue;

        value = query.value(query.record().indexOf(columnName)).toString().toFloat(&ok);
        if (!ok)
            value = std::numeric_limits<float>::quiet_NaN();
        else
        {
            if (value < min)
                min = value;
            if (value > max)
                max = value;
        }
        point.setX(i);
        point.setY(value);

        points.push_back(point);
        comments.push_back(query.value(query.record().indexOf("comment")).toString());
        i++;
    }

    curve.setMin(min);
    curve.setMax(max);

    curve.setName(columnName + " weight");
    curve.setPoints(points);
    curve.setComments(comments);
}
