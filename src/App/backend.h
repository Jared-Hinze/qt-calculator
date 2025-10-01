#pragma once

#include <QObject>
#include <QTimer>

class Backend : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString display READ display NOTIFY displayChanged)
	Q_PROPERTY(bool locked READ locked NOTIFY lockedChanged)

public:
	explicit Backend(QObject *parent = nullptr);

	// Views
	QString display() const;
	bool locked() const;

	// Helpers
	void setLocked(bool locked);
	void replace(QString txt);

public slots:
	QString calculate(QString s);
	void updateDisplay(const QString &btn);

signals:
	void displayChanged(const QString &s);
	void lockedChanged(const bool);

private:
	QString m_display;	// track QML display
	QString m_num;		// track current "number" for buttons like 1/x or CE
	bool m_locked;		// track error state
};
