#include "libbremsweg.h"
/**
 * @brief Bremsweg::Bremsweg
 *        Default constructor for the class Bremsweg
 * @param none
 * @return none
 */
Bremsweg::Bremsweg()
:untergrundfaktor(1.0), geschwindigkeit(0.0), bremsweg(0.0), gefahrenbremsung (1), fahrzeug("PkW")
{
}

/**
 * @brief Bremsweg::Bremsweg
 *        Constructor used in the Testprogramm
 * @param untergrundfaktor (double); Value depending on the underground
 * @param geschwindigkeit (double);  Speed of the vehicle
 * @param gefahrenbremsung (int); Value of the emergency brake
 * @param fahrzeug (string); Type of vehicle
 */
Bremsweg::Bremsweg(double untergrundfaktor, double geschwindigkeit, int gefahrenbremsung, std::string fahrzeug)
    :untergrundfaktor(untergrundfaktor), geschwindigkeit(geschwindigkeit), gefahrenbremsung(gefahrenbremsung),fahrzeug(fahrzeug)
{
}

/**
 * @brief Bremsweg::GeschwindigkeitSetzen
 *        Changes the speed of the vehicle
 * @param neueGeschwindigkeit (double); New speed of the vehicle
 */
void Bremsweg::GeschwindigkeitSetzen(double neueGeschwindigkeit)
{
    if (geschwindigkeit != neueGeschwindigkeit)
    {
        geschwindigkeit = neueGeschwindigkeit;
        emit WertGeaendert();
    }
}

/**
 * @brief Bremsweg::UntergrundfaktorSetzen
 *        Changes the factor of the underground
 * @param neuerUntergrundfaktor (double); Value of the new underground
 */
void Bremsweg::UntergrundfaktorSetzen(double neuerUntergrundfaktor)
{
    if (untergrundfaktor != neuerUntergrundfaktor)
    {
        untergrundfaktor = neuerUntergrundfaktor;
        emit WertGeaendert();
    }
}

/**
 * @brief Bremsweg::GefahrenbremsungSetzen
 *        Changes the state of the emergency brake
 * @param Gefahrenbremsung (int); Changes state of the emergency brake
 */
void Bremsweg::GefahrenbremsungSetzen(int Gefahrenbremsung)
{
    if (gefahrenbremsung != Gefahrenbremsung)
    {
        gefahrenbremsung = Gefahrenbremsung;
        emit WertGeaendert();
    }
}

/**
 * @brief Bremsweg::FahrzeugSetzen
 *        Changes the type of vehicle
 * @param neuesFahrzeug (string); New type of vehicle
 */
void Bremsweg::FahrzeugSetzen(std::string neuesFahrzeug)
{
    if (fahrzeug != neuesFahrzeug)
    {
        fahrzeug = neuesFahrzeug;
        emit WertGeaendert();
    }
}

/**
 * @brief Bremsweg::BremswegBerechnen
 *        Calculates the braking distance of the vehicle, depending on the user-choices currently saved in the class
 */
void Bremsweg::BremswegBerechnen()
{
    double NORMFALLBESCHLEUNIGUNG = 9.80665;
    double Haftreibungszahl = 0.0;
    double bremsbeschleunigung = 0.0;
    double bremszeit = 0.0;
    double reaktionsweg = geschwindigkeit/3.6;
    if (fahrzeug == "PkW")
    {
        Haftreibungszahl = 0.8;
        bremsbeschleunigung = Haftreibungszahl * NORMFALLBESCHLEUNIGUNG;
        bremszeit = (geschwindigkeit/3.6)/bremsbeschleunigung;
        if (gefahrenbremsung == 2)
        {
            bremsweg = (round((reaktionsweg+0.5*bremsbeschleunigung*bremszeit*bremszeit)*untergrundfaktor*10))/10;
        }
        else
        {
            bremsweg = (round((reaktionsweg+0.5*bremsbeschleunigung*bremszeit*bremszeit)*untergrundfaktor*15))/10;
        }
    }
    else if (fahrzeug == "Panzer")
    {
        Haftreibungszahl = 1.51;
        bremsbeschleunigung = Haftreibungszahl * Normfallbeschleunigung;
        bremszeit = (geschwindigkeit/3.6)/bremsbeschleunigung;
        bremsweg = (round((reaktionsweg+0.5*bremsbeschleunigung*bremszeit*bremszeit)*untergrundfaktor*10))/10;
    }
    else if (fahrzeug == "Zug")
    {
        Haftreibungszahl = 0.18;
        bremsbeschleunigung = Haftreibungszahl * Normfallbeschleunigung;
        bremszeit = (geschwindigkeit/3.6)/bremsbeschleunigung;
        bremsweg = (round((reaktionsweg+0.5*bremsbeschleunigung*bremszeit*bremszeit)*untergrundfaktor*10))/10;
    }
}

/**
 * @brief Bremsweg::BremswegAbfragen
 *        Returns the braking distance
 * @return bremsweg (double); calculated braking distance
 */
double Bremsweg::BremswegAbfragen() const
{
    return bremsweg;
}
