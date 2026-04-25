#include <QObject>
#include <QtTest>

class TestDownsampler : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testPlaceholder();
    void cleanupTestCase();
};

void TestDownsampler::initTestCase() {}

void TestDownsampler::testPlaceholder()
{
    QVERIFY(true);
}

void TestDownsampler::cleanupTestCase() {}

QTEST_MAIN(TestDownsampler)
#include "tst_downsampler.moc"
