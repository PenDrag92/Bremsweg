#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libbremsweg.h"
#include <QMessageBox>
#include <QComboBox>

bool StartupFahrzeug = true; //verhindert das beim starten des Programms direkt eine Fehlermeldung kommt
bool StartupUntergrund = true; //verhindert das beim starten des Programms direkt eine Fehlermeldung kommt
bool gueltigesFahrzeug = false;
bool gueltigerUntergrund = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBoxFahrzeug->addItem("Fahrzeugauswahl");
    ui->comboBoxFahrzeug->addItem("PkW");
    ui->comboBoxFahrzeug->addItem("Zug");
    ui->comboBoxFahrzeug->addItem("Panzer");

    ui->comboBoxUntergrund->addItem("Untergrundauswahl");
    ui->comboBoxUntergrund->addItem("trocken");
    ui->comboBoxUntergrund->addItem("nass");
    ui->comboBoxUntergrund->addItem("verschneit");

    connect(bremsweg, &Bremsweg::WertGeaendert,
        bremsweg, &Bremsweg::BremswegBerechnen);//bei einer Wertänderung wird automatisch der neue Bremsweg berechnet
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bremsweg;
    delete ERROR_FAHRZEUG;
    delete ERROR_GESCHWINDIGKEIT_INVALID;
    delete ERROR_GESCHWINDIGKEIT_NUMMER;
    delete ERROR_UNTERGRUND;
}

/**
 * @brief MainWindow::on_pushButtonBremswegberechnen_clicked
 *        If the Button "Bremsweg berechnen" is clicked the programm will calculate the stopping distance of the vehicle with the given type, underground and speed
 * @param bremsweg(double); Currently calculated stopping distance, according to class
 * @return AusgabeBremsweg (string); Stopping distance, according to class, to display in LCD-Display
 */

void MainWindow::on_pushButtonBremswegberechnen_clicked()//"Bremsweg berechnen" geklicked
{
    bool ok = false;
    QString AusgabeBremsweg ="";
    QString EingabeGeschwindigkeit = ui->lineEditGeschwindigkeit->text();
    double Geschwindigkeit = EingabeGeschwindigkeit.toDouble(&ok);
    if (gueltigesFahrzeug == true && gueltigerUntergrund == true) //gültiger Untergrund und gültiges Fahrzeug
    {
        if (ok == true) //Umwandlung nach Double ok
        {
            if (Geschwindigkeit > 0 && Geschwindigkeit <= 250)//gültige Geschwindigkeit
            {
                this -> bremsweg -> GeschwindigkeitSetzen(Geschwindigkeit);
                double bremsweg = this ->bremsweg->BremswegAbfragen();
                AusgabeBremsweg = QString::number(bremsweg);
                ui -> lcdBremsweg -> display(AusgabeBremsweg);
            }
            else //Geschwindigkeit negativ oder > 250 km/h
            {
                AusgabeBremsweg = QString(ERROR_GESCHWINDIGKEIT_INVALID);
                ui -> lcdBremsweg -> display("ERROR");
                QMessageBox::information(this, "Achtung", AusgabeBremsweg);
            }
        }
        else //keine Zahl bei Geschwindigkeit eingegeben
        {
            AusgabeBremsweg = QString(ERROR_GESCHWINDIGKEIT_NUMMER);
            ui->lcdBremsweg->display("ERROR");
            QMessageBox::information(this, "Achtung", AusgabeBremsweg);
        }
    }
    else if (gueltigesFahrzeug == true && gueltigerUntergrund != true) //kein Untergrund ausgewählt
    {
        AusgabeBremsweg = QString(ERROR_UNTERGRUND);
        ui->lcdBremsweg->display("ERROR");
        QMessageBox::information(this, "Achtung", AusgabeBremsweg);
    }
    else if (gueltigesFahrzeug != true && gueltigerUntergrund == true) //kein Fahrzeug ausgewählt
    {
        AusgabeBremsweg = QString(ERROR_FAHRZEUG);
        ui->lcdBremsweg->display("ERROR");
        QMessageBox::information(this, "Achtung", AusgabeBremsweg);
    }
    else //kein Fahrzeug und kein Untergrund ausgewählt
    {
        AusgabeBremsweg = QString(ERROR_FAHRZEUGundUNTERGRUND);
        ui->lcdBremsweg->display("ERROR");
        QMessageBox::information(this, "Achtung", AusgabeBremsweg);
    }
}

/**
 * @brief MainWindow::on_checkBoxGefahrenbremsung_stateChanged
 *        Checkbox to activate the emergency brake if the vehicle is a car
 *        If checked emergency brake is on
 * @param Gefahrenbremsung (int); Current status of the Checkbox
 * @return GefahrenbremsungSetzen (int); Value returned to the class, depending on the status of the Checkbox
 */
void MainWindow::on_checkBoxGefahrenbremsung_stateChanged(int Gefahrenbremsung) //Zustand der Gefahrenbremsung geändert
{
    if (Gefahrenbremsung) //Haken bei Gefahrenbremsung gesetzt
    {
        QMessageBox::information(this, "Achtung", "Die Gefahrenbremsung wurde aktiviert, wird jedoch nur beim Auto berücksichtigt!");
        this -> bremsweg -> GefahrenbremsungSetzen(2);
    }
    else //Haken bei Gefahrenbremsung entfernt
    {
        this -> bremsweg -> GefahrenbremsungSetzen(1);
    }
}

/**
 * @brief MainWindow::on_comboBoxFahrzeug_currentIndexChanged
 *        Combobox to change the type of vehicle
 * @param Fahrzeug (string); The string currently displayed in the Combobox
 * @param gueltigesFahrzeug (bool); Global variable to prevent an error when starting the programm
 * @return fahrzeug (string); String returned to the class, depending on the selected vehicle
 */
void MainWindow::on_comboBoxFahrzeug_currentIndexChanged(const QString &Fahrzeug) //Fahrzeugauswahl geändert
{
    std::string fahrzeug = "";
    if (Fahrzeug == "Zug")
    {
        gueltigesFahrzeug = true;
        fahrzeug = "Zug";
    }
    else if (Fahrzeug == "PkW")
    {
        gueltigesFahrzeug = true;
        fahrzeug = "PkW";
    }
    else if (Fahrzeug == "Panzer")
    {
        gueltigesFahrzeug = true;
        fahrzeug = "Panzer";
    }
    else
    {
        if (StartupFahrzeug != true) //wenn zurückgewechselt wird auf eintrag "Fahrzeugauswahl" kommt eine Fehlermeldung, beim starten jedoch nicht
        {
            gueltigesFahrzeug = false;
            QMessageBox::information(this, "Achtung", "Bitte ein gültiges Fahrzeug auswählen");
        }
    }
    StartupFahrzeug = false;//nach dem ersten Mal wechseln wird der "Startmodus" verlassen, ein wechsel auf "Fahrzeugauswahl" bringt jetzt eine Fehlermeldung
    if (fahrzeug != "")
    {
        this -> bremsweg -> FahrzeugSetzen(fahrzeug);
    }
}

/**
 * @brief MainWindow::on_comboBoxUntergrund_currentIndexChanged
 *        Combobox to change the type of underground
 * @param Untergrund (string); The string currently displayed in the Combobox
 * @param gueltigerUntergrund (bool); Global variable to prevent an error when starting the programm
 * @return untergrundfaktor (double); Value returned to the class, depending on the selected underground
 */
void MainWindow::on_comboBoxUntergrund_currentIndexChanged(const QString &Untergrund)//Untergrundauswahl geändert
{
    double untergrundfaktor = 0.0;
    if (Untergrund == "trocken")
    {
        gueltigerUntergrund = true;
         untergrundfaktor = 1;
    }
    else if (Untergrund == "nass")
    {
        gueltigerUntergrund = true;
        untergrundfaktor = 1.1;
    }
    else if (Untergrund == "verschneit")
    {
        gueltigerUntergrund = true;
        untergrundfaktor = 2.5;
    }
    else
    {
        if (StartupUntergrund != true)//wenn zurückgewechselt wird auf eintrag "Untergrundauswahl" kommt eine Fehlermeldung, beim starten jedoch nicht
        {
            gueltigerUntergrund = false;
            QMessageBox::information(this, "Achtung", "Bitte einen gültigen Untergrund auswählen");
        }
    }
    StartupUntergrund = false;//nach dem ersten Mal wechseln wird der "Startmodus" verlassen, ein wechsel auf "Untergrundauswahl" bringt jetzt eine Fehlermeldung
    if (untergrundfaktor != 0)
    {
        this -> bremsweg -> UntergrundfaktorSetzen(untergrundfaktor);
    }
}
