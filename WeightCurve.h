#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QUrl>
#include <QVector>
#include <QDate>
#include <QTimer>
#include <QPoint>
#include <QColor>
#include <QAbstractListModel>

class Curve : public QObject    // TODO add comments property (one per day, not linked to points, a QStringList should be fine, with some empty)
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QVariantList points READ points NOTIFY pointsChanged)
    Q_PROPERTY(QStringList comments READ comments WRITE setComments NOTIFY commentsChanged)
    Q_PROPERTY(float min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(float max READ max WRITE setMax NOTIFY maxChanged)

    void            setName(const QString& name) {mName = name; emit nameChanged(mName);}
    QString         name() const {return mName;}

    void            setColor(const QColor& color) {mColor = color; emit colorChanged(mColor);}
    QColor          color() const {return mColor;}

    void            setPoints(const QVariantList& points) {mPoints = points; emit pointsChanged(mPoints);}
    QVariantList    points() const {return mPoints;}

    void            setComments(const QStringList& comments) {mComments = comments; emit commentsChanged(mComments);}
    QStringList     comments() const {return mComments;}

    void            setMin(float min) {mMin = min; emit minChanged(mMin);}
    float           min() const {return mMin;}

    void            setMax(float max) {mMax = max; emit maxChanged(mMax);}
    float           max() const {return mMax;}

signals:
    void    nameChanged(const QString&);
    void    colorChanged(const QColor&);
    void    pointsChanged(const QVariantList&);
    void    commentsChanged(const QStringList&);
    void    minChanged(float);
    void    maxChanged(float);

private:
    QString         mName;
    QColor          mColor;
    QVariantList    mPoints;
    QStringList     mComments;
    float           mMin;
    float           mMax;
};

class WeightCurve : public QObject
{
    Q_OBJECT

    static QString      dbFileName;
    static WeightCurve* instance;

public:
    static WeightCurve* singleton();
    static void         destroy();

    void    initialize();

    Q_PROPERTY(float morningWeight READ morningWeight WRITE setMorningWeight NOTIFY morningWeightChanged)
    Q_PROPERTY(float noonWeight READ noonWeight WRITE setNoonWeight NOTIFY noonWeightChanged)
    Q_PROPERTY(float eveningWeight READ eveningWeight WRITE setEveningWeight NOTIFY eveningWeightChanged)

    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)

    Q_PROPERTY(QObject* morningCurve READ morningCurve NOTIFY morningCurveChanged)
    Q_PROPERTY(QObject* noonCurve READ noonCurve NOTIFY noonCurveChanged)
    Q_PROPERTY(QObject* eveningCurve READ eveningCurve NOTIFY eveningCurveChanged)

    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)

    Q_INVOKABLE bool    importFromCSVFile(const QUrl& sourceFilePath);
    Q_INVOKABLE bool    exportToCSVFile(const QUrl& destinationFilePath);

    void    setMorningWeight(float weight);
    float   morningWeight() const;
    void    setNoonWeight(float weight);
    float   noonWeight() const;
    void    setEveningWeight(float weight);
    float   eveningWeight() const;

    void    setComment(const QString& comment);
    QString comment() const;

    Curve*  morningCurve();
    Curve*  noonCurve();
    Curve*  eveningCurve();

    void    setStartDate(const QDate&);
    QDate   startDate() const;
    void    setEndDate(const QDate&);
    QDate   endDate() const;

signals:
    void    dateChanged();  /// Called when current day changed

    void    morningWeightChanged(float weight);
    void    noonWeightChanged(float weight);
    void    eveningWeightChanged(float weight);

    void    commentChanged(const QString& comment);

    void    morningCurveChanged(Curve*);
    void    noonCurveChanged(Curve*);
    void    eveningCurveChanged(Curve*);

    void    startDateChanged(const QDate& date);
    void    endDateChanged(const QDate& date);

private slots:
    void    onTimerUpdated();
    void    onDateChanged();

private:
    WeightCurve();
    ~WeightCurve();

    void    updateCurves(); // startDate and enDate must be valid
    void    updateCurve(const QString& columnName, Curve& curve);

    QString         mDBFilePath;
    QSqlDatabase    mDB;
    QTimer          mTimer;
    Curve           mMorningCurve;
    Curve           mNoonCurve;
    Curve           mEveningCurve;
    QDate           mStartDate;
    QDate           mEndDate;
};
