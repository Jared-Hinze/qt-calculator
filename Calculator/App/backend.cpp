#include "backend.h"
#include <cmath>
#include <QDebug>
#include <QJSEngine>
#include <QJSValue>
#include <QRegularExpression>

// =============================================================================
Backend::Backend(QObject *parent)
    : QObject{parent},
    m_display(QString("0")),
    m_num(QString("")),
    m_locked(false)
{

}

// -----------------------------------------------------------------------------
QString Backend::display() const
{
    return m_display;
}

// -----------------------------------------------------------------------------
bool Backend::locked() const
{
    return m_locked;
}

// -----------------------------------------------------------------------------
void Backend::setLocked(bool locked)
{
    /* Sets m_locked to and emits lockedChanged so the UI knows an error has
       occurred. This includes technically valid JS expressions that evaluate
       to non-numeric values. */
    if (m_locked != locked)
    {
        m_locked = locked;
        emit lockedChanged(m_locked);
    }
}

// -----------------------------------------------------------------------------
QString Backend::calculate(QString s)
{
    /* Perform the calculation of the expression shown in the m_display. This
       uses QJSEngine to effectively "eval" the string in JS which reduces the
       complexity of creating my own parsing logic. It also implicitly handles
       operator precedence.

       Note 1:
            The "eval" is safe since all possible inputs are known.
       Note 2:
            The app currently doesn't support keyboard inputs so while the
            QJSEngine would support scientific notation and parentheses the app
            will not concern itself with those. */
    qDebug() << "Calculating " << s;

    s = s.replace("÷", "/")     // JS divide
            .replace("×", "*")      // JS multiply
            .replace("--", "- -");  // x- (-x)

    QJSEngine engine;
    QJSValue result = engine.evaluate(s);

    if (result.isError())
    {
        return result.property("name").toString();
    }
    else
    {
        QString value = result.toString();
        bool valid;
        value.toDouble(&valid);
        setLocked(!valid);
        return value;
    }
}

// -----------------------------------------------------------------------------
void Backend::updateDisplay(const QString &btn)
{
    /* The primary driving function to handle updating the QML display. Each
       button is handled separately with limited error handling largely relying
       on the QJSEngine to notify us when something goes wrong. */
    qDebug() << "updateDisplay " << btn;

    // Bail if locked and no trying to clear
    if (m_locked && btn != "C") {
        return;
    }

    QStringList ops = {"÷", "×", "-", "+"};

    // Calculate the answer
    if (btn == "=")
    {
        m_display = calculate(m_display);
        m_num = m_display;
    }
    // Create a percentage out of the current RHS m_num
    else if (btn == "%")
    {
        QRegularExpression re("[" + ops.join("") + "]");
        if (m_display.contains(re) && !m_num.isEmpty())
        {
            replace(QString::number(m_num.toDouble() / 100.0, 'f'));
        }
    }
    // Clear last number entered (m_num)
    else if (btn == "CE")
    {
        m_display.chop(m_num.size());
        if (m_display.isEmpty())
        {
            m_display = "0";
        }
        m_num = "";
    }
    // Clear entire calculation (m_display and m_num)
    else if (btn == "C")
    {
        m_display = "0";
        m_num = "";
        setLocked(false);
    }
    // Backspace, but keep 0 if it's the only thing left
    else if (btn == "DEL" && m_display != "0")
    {
        m_display.chop(1);
        m_num.chop(1);
    }
    // Convert m_num to a fraction
    else if (btn == "1/x")
    {
        if (m_num == "0")
        {
            m_display = "ERROR DIV 0";
            m_num.clear();
            setLocked(true);
        }
        else
        {
            replace(QString::number(std::pow(m_num.toDouble(), -1), 'f'));
        }
    }
    // Square m_num
    else if (btn == "x²")
    {
        replace(QString::number(std::pow(m_num.toDouble(), 2), 'f'));
    }
    // Square root m_num
    else if (btn == "√x")
    {
        if (m_num.startsWith("-"))
        {
            m_display = "ERROR COMPLEX";
            m_num.clear();
            setLocked(true);
        }
        else
        {
            replace(QString::number(std::sqrt(m_num.toDouble()), 'f'));
        }
    }
    // Invert sign of m_num
    else if (btn == "+/-")
    {
        if (m_num.startsWith("-"))
        {
            replace(m_num.mid(1));
        }
        else
        {
            replace("-" + m_num);
        }
    }
    // Reset m_num since we're on to a new operator
    else if (ops.contains(btn)) {
        m_display += btn;
        m_num = "";
    }
    // Set m_num as long as it's not '0' or a '.' for first character
    else if (m_display == "0" && btn != "0" && btn != ".")
    {
        m_display = btn;
        m_num = btn;
    }
    // Update m_display and m_num with btn
    else
    {
        m_display += btn;
        m_num += btn;
    }

    qDebug() << "===> m_display: " << m_display << " m_num: " << m_num;

    // Actually update the QML display
    emit displayChanged(m_display);
}

// -----------------------------------------------------------------------------
void Backend::replace(QString txt)
{
    /* A convenience function to manage updating m_display and m_num with
       consideration for trailing 0's and whole number adjustments. */

    // Cut trailing 0's and the dot if txt is whole
    while (txt.contains(".") && txt.endsWith("0"))
    {
        txt.chop(1);
    }
    if (txt.endsWith("."))
    {
        txt.chop(1);
    }

    // Update m_display and m_num so calculations can continue
    m_display.replace(
        m_display.lastIndexOf(m_num),
        m_num.length(),
        txt
        );
    m_num = txt;
}
