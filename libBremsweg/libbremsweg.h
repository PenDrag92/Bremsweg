#include <QObject>

#ifndef LIBBREMSWEG_H
#define LIBBREMSWEG_H

class Bremsweg : public QObject
{
    Q_OBJECT

public:
    Bremsweg();//wird normal im Programm benutzt
    Bremsweg(double untergrundfaktor, double geschwindigkeit, int gefahrenbremsung, std::string fahrzeug);//wird für die Test-Cases benutzt
    double BremswegAbfragen() const;
    void BremswegBerechnen();

public slots:
    void GeschwindigkeitSetzen(double neueGeschwindigkeit);
    void UntergrundfaktorSetzen(double neuerUntergrundfaktor);
    void GefahrenbremsungSetzen (int Gefahrenbremsung);
    void FahrzeugSetzen (std::string neuesFahrzeug);

signals:
    void WertGeaendert();//wird emittet wenn ein Wert geändert wurde, durch connect verbunden mit BremswegBerechnen, dadurch wird direkt der neue Bremsweg berechnet intern.

private:
    double untergrundfaktor;
    double geschwindigkeit;
    double bremsweg;
    int gefahrenbremsung;
    std::string fahrzeug;
};

#endif // LIBBREMSWEG_H
