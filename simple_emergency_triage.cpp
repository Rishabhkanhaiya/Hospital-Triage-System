#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdexcept>

using namespace std;

// ==========================================
// Part 1: Vitals Class
// ==========================================
class Vitals {
private:
    int heartRate;
    int systolicBP;
    int spo2;

public:
    Vitals() : heartRate(75), systolicBP(120), spo2(98) {}

    Vitals(int hr, int bp, int o2) {
        setHeartRate(hr);
        setSystolicBP(bp);
        setSpo2(o2);
    }

    int getHeartRate() const { return heartRate; }
    int getSystolicBP() const { return systolicBP; }
    int getSpo2() const { return spo2; }

    void setHeartRate(int hr) {
        if (hr < 0 || hr > 300) {
            throw invalid_argument("Heart Rate must be between 0 and 300 bpm.");
        }
        heartRate = hr;
    }

    void setSystolicBP(int bp) {
        if (bp < 0 || bp > 300) {
            throw invalid_argument("Blood Pressure must be between 0 and 300 mmHg.");
        }
        systolicBP = bp;
    }

    void setSpo2(int o2) {
        if (o2 < 0 || o2 > 100) {
            throw invalid_argument("SpO2 oxygen saturation must be between 0 and 100%.");
        }
        spo2 = o2;
    }

    void display() const {
        cout << "Heart Rate: " << heartRate << " bpm | BP: " << systolicBP 
             << " mmHg | SpO2: " << spo2 << "%";
    }
};

// ==========================================
// Part 2: Person Class
// ==========================================
class Person {
protected:
    string id;
    string name;
    int age;
    string phone;

public:
    Person(string id, string name, int age, string phone)
        : id(id), name(name), age(age), phone(phone) {}

    virtual ~Person() {}

    string getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getPhone() const { return phone; }

    virtual void displayDetails() const {
        cout << "ID: " << id << " | Name: " << name 
             << " | Age: " << age << " | Phone: " << phone << "\n";
    }
};

// ==========================================
// Part 3: Patient Class
// ==========================================
class Patient : public Person {
protected:
    string chiefComplaint;
    Vitals vitals;
    int priorityLevel;
    int arrivalOrder;

public:
    Patient(string id, string name, int age, string phone, string complaint, Vitals v, int order = 0)
        : Person(id, name, age, phone), chiefComplaint(complaint), vitals(v), priorityLevel(3), arrivalOrder(order) {}

    virtual ~Patient() {}

    Vitals getVitals() const { return vitals; }
    int getPriorityLevel() const { return priorityLevel; }
    string getComplaint() const { return chiefComplaint; }

    void setPriorityLevel(int p) { priorityLevel = p; }
    void setArrivalOrder(int order) { arrivalOrder = order; }
    void updateVitals(const Vitals& v) { vitals = v; }

    string getPriorityString() const {
        if (priorityLevel == 1) return "[LEVEL 1 - CRITICAL (RED)]";
        if (priorityLevel == 2) return "[LEVEL 2 - URGENT (YELLOW)]";
        return "[LEVEL 3 - NORMAL (GREEN)]";
    }

    virtual void displayDetails() const override {
        Person::displayDetails();
        cout << "   Complaint: " << chiefComplaint << "\n";
        cout << "   Priority : " << getPriorityString() << "\n";
        cout << "   Vitals   : ";
        vitals.display();
        cout << "\n";
    }

    bool operator<(const Patient& other) const {
        if (this->priorityLevel != other.priorityLevel) {
            return this->priorityLevel < other.priorityLevel;
        }
        return this->arrivalOrder < other.arrivalOrder;
    }
};

// ==========================================
// Part 4: Specialized Patient Classes
// ==========================================
class CardiacPatient : public Patient {
private:
    bool severeChestPain;
    string ecgStatus;

public:
    CardiacPatient(string id, string name, int age, string phone, string complaint, 
                   Vitals v, bool chestPain, string ecg, int order = 0)
        : Patient(id, name, age, phone, complaint, v, order), 
          severeChestPain(chestPain), ecgStatus(ecg) {}

    virtual void displayDetails() const override {
        Patient::displayDetails();
        cout << "   [Cardiac Info] Chest Pain: " << (severeChestPain ? "YES" : "No")
             << " | ECG Status: " << ecgStatus << "\n";
    }
};

class TraumaPatient : public Patient {
private:
    string injuryType;
    bool activeBleeding;

public:
    TraumaPatient(string id, string name, int age, string phone, string complaint, 
                  Vitals v, string injury, bool bleeding, int order = 0)
        : Patient(id, name, age, phone, complaint, v, order), 
          injuryType(injury), activeBleeding(bleeding) {}

    virtual void displayDetails() const override {
        Patient::displayDetails();
        cout << "   [Trauma Info] Injury: " << injuryType 
             << " | Active Bleed: " << (activeBleeding ? "YES (High Risk)" : "No") << "\n";
    }
};

// ==========================================
// Part 5: Hospital System
// ==========================================
class HospitalSystem {
private:
    vector<Patient*> waitingQueue;
    int patientCounter;

public:
    HospitalSystem() : patientCounter(0) {}

    ~HospitalSystem() {
        for (Patient* p : waitingQueue) {
            delete p;
        }
        waitingQueue.clear();
    }

    int calculatePriority(const Vitals& v) {
        if (v.getSpo2() < 88 || v.getHeartRate() > 130 || v.getHeartRate() < 45 || v.getSystolicBP() < 80) {
            return 1;
        }
        if (v.getSpo2() < 94 || v.getHeartRate() > 100 || v.getSystolicBP() > 160 || v.getSystolicBP() < 90) {
            return 2;
        }
        return 3;
    }

    void registerPatient(Patient* p) {
        patientCounter++;
        p->setArrivalOrder(patientCounter);

        int priority = calculatePriority(p->getVitals());
        p->setPriorityLevel(priority);

        waitingQueue.push_back(p);
        sortQueue();

        cout << "\n>>> [PATIENT REGISTERED SUCCESSFULLY] <<<\n";
        cout << "Name: " << p->getName() << " | Assigned Priority: " << p->getPriorityString() << "\n";
    }

    void sortQueue() {
        sort(waitingQueue.begin(), waitingQueue.end(), [](Patient* a, Patient* b) {
            return (*a) < (*b);
        });
    }

    void displayQueue() const {
        cout << "\n====================================================================================\n";
        cout << "                       EMERGENCY ROOM LIVE TRIAGE QUEUE BOARD                       \n";
        cout << "====================================================================================\n";
        if (waitingQueue.empty()) {
            cout << "              [ No patients currently waiting. Waiting room is clear! ]             \n";
            cout << "====================================================================================\n";
            return;
        }

        int rank = 1;
        for (const Patient* p : waitingQueue) {
            cout << "Rank #" << rank++ << " : " << p->getName() 
                 << " (ID: " << p->getId() << ", Age: " << p->getAge() << ")\n";
            cout << "         " << p->getPriorityString() 
                 << " | Complaint: " << p->getComplaint() << "\n";
            cout << "         Vitals: ";
            p->getVitals().display();
            cout << "\n------------------------------------------------------------------------------------\n";
        }
    }

    void admitNextPatient() {
        if (waitingQueue.empty()) {
            cout << "\n[!] Waiting queue is empty. No patient to call.\n";
            return;
        }

        Patient* nextPatient = waitingQueue.front();
        waitingQueue.erase(waitingQueue.begin());

        cout << "\n======================================================================\n";
        cout << "               >>> CALLING NEXT PATIENT TO DOCTOR BAY <<<             \n";
        cout << "======================================================================\n";
        nextPatient->displayDetails();
        cout << "Action: Patient transferred to Doctor Consultation / Trauma Bay.\n";
        cout << "======================================================================\n";

        delete nextPatient;
    }

    void reTriagePatient(string id, int newHR, int newBP, int newSpO2) {
        for (Patient* p : waitingQueue) {
            if (p->getId() == id) {
                Vitals newV(newHR, newBP, newSpO2);
                p->updateVitals(newV);

                int oldPriority = p->getPriorityLevel();
                int newPriority = calculatePriority(newV);
                p->setPriorityLevel(newPriority);

                sortQueue();

                cout << "\n>>> [RE-TRIAGE COMPLETED] <<<\n";
                cout << "Patient: " << p->getName() << " (ID: " << id << ")\n";
                cout << "Previous Priority: Level " << oldPriority 
                     << " --> New Priority: " << p->getPriorityString() << "\n";
                cout << "Queue has been automatically re-ordered by priority!\n";
                return;
            }
        }
        cout << "\n[!] Patient ID '" << id << "' not found in the waiting list.\n";
    }

    void saveReportToFile(string filename = "triage_report.txt") const {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "\n[!] Error opening file for writing.\n";
            return;
        }

        outFile << "======================================================================\n";
        outFile << "               HOSPITAL EMERGENCY TRIAGE AUDIT REPORT                 \n";
        outFile << "======================================================================\n\n";

        if (waitingQueue.empty()) {
            outFile << "No patients currently in queue.\n";
        } else {
            int rank = 1;
            for (const Patient* p : waitingQueue) {
                outFile << "Rank #" << rank++ << " | ID: " << p->getId() 
                        << " | Name: " << p->getName() << " | Age: " << p->getAge() << "\n";
                outFile << "Priority : " << p->getPriorityString() << "\n";
                outFile << "Complaint: " << p->getComplaint() << "\n";
                outFile << "Vitals   : HR=" << p->getVitals().getHeartRate() 
                        << " bpm, BP=" << p->getVitals().getSystolicBP() 
                        << " mmHg, SpO2=" << p->getVitals().getSpo2() << "%\n";
                outFile << "----------------------------------------------------------------------\n";
            }
        }

        outFile.close();
        cout << "\n>>> Triage report successfully saved to '" << filename << "' <<<\n";
    }

    void loadSamplePatients() {
        Vitals v1(72, 118, 99);
        registerPatient(new Patient("P101", "Siddharth Verma", 29, "+91-9876500001", "Mild ankle sprain", v1));

        Vitals v2(108, 125, 93);
        registerPatient(new Patient("P102", "Ananya Sen", 24, "+91-9876500002", "High fever with breathing difficulty", v2));

        Vitals v3(140, 75, 87);
        registerPatient(new CardiacPatient("P103", "Rajesh Sharma", 58, "+91-9876500003", 
                                           "Crushing chest pain radiating to left arm", v3, true, "ST Elevation Present"));

        Vitals v4(135, 78, 86);
        registerPatient(new TraumaPatient("P104", "Vikram Malhotra", 32, "+91-9876500004", 
                                          "Bike accident with severe bleeding", v4, "Compound Fracture", true));

        cout << "\n>>> 4 Sample patients loaded! Open the queue to see them sorted by priority! <<<\n";
    }
};

// ==========================================
// Part 6: Main Function
// ==========================================
int main() {
    HospitalSystem hospital;
    int choice;

    cout << "\n======================================================================\n";
    cout << "              HOSPITAL EMERGENCY TRIAGE SYSTEM (OOPS PROJECT)          \n";
    cout << "======================================================================\n";

    while (true) {
        cout << "\n---------------------------- MENU OPTIONS ----------------------------\n";
        cout << " 1. [DEMO] Load 4 Sample Patients (Fast Viva Demo)\n";
        cout << " 2. Register New Patient (Manual Entry)\n";
        cout << " 3. Show Live Waiting Queue (Sorted by Priority)\n";
        cout << " 4. Admit / Call Next Patient to Doctor Bay\n";
        cout << " 5. Dynamic Re-Triage (Simulate Patient Condition Getting Worse)\n";
        cout << " 6. Save Triage Report to Text File (File Handling)\n";
        cout << " 7. Future Scope & Next Steps\n";
        cout << " 0. Exit System\n";
        cout << "----------------------------------------------------------------------\n";
        cout << " Enter choice (0-7): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] Please enter a valid number.\n";
            continue;
        }

        if (choice == 0) {
            cout << "\nExiting system. Best of luck with your evaluation!\n";
            break;
        }

        switch (choice) {
            case 1:
                hospital.loadSamplePatients();
                hospital.displayQueue();
                break;

            case 2: {
                try {
                    string id, name, phone, complaint;
                    int age, typeChoice, hr, bp, o2;

                    cout << "\nSelect Patient Type (1: General, 2: Cardiac, 3: Trauma): ";
                    cin >> typeChoice;
                    cin.ignore();

                    cout << "Enter Patient ID (e.g., P105): ";
                    getline(cin, id);
                    cout << "Enter Full Name: ";
                    getline(cin, name);
                    cout << "Enter Age: ";
                    cin >> age;
                    cin.ignore();
                    cout << "Enter Contact Phone: ";
                    getline(cin, phone);
                    cout << "Enter Chief Complaint: ";
                    getline(cin, complaint);

                    cout << "\nEnter Vital Signs:\n";
                    cout << "  Heart Rate (bpm, e.g. 80): "; cin >> hr;
                    cout << "  Blood Pressure (Systolic, e.g. 120): "; cin >> bp;
                    cout << "  SpO2 Oxygen Saturation (% e.g. 98): "; cin >> o2;
                    cin.ignore();

                    Vitals v(hr, bp, o2);

                    if (typeChoice == 2) {
                        hospital.registerPatient(new CardiacPatient(id, name, age, phone, complaint, v, true, "Normal"));
                    } else if (typeChoice == 3) {
                        hospital.registerPatient(new TraumaPatient(id, name, age, phone, complaint, v, "Blunt Trauma", false));
                    } else {
                        hospital.registerPatient(new Patient(id, name, age, phone, complaint, v));
                    }
                } catch (const exception& e) {
                    cout << "\n[ERROR]: " << e.what() << "\n";
                }
                break;
            }

            case 3:
                hospital.displayQueue();
                break;

            case 4:
                hospital.admitNextPatient();
                break;

            case 5: {
                string pid;
                int hr, bp, o2;
                cout << "\nEnter Patient ID to Re-Triage (e.g. P101): ";
                cin >> pid;
                cout << "\n--- SIMULATE PATIENT CONDITION GETTING WORSE ---\n";
                cout << "Enter New Heart Rate (e.g. 145 for tachycardia): "; cin >> hr;
                cout << "Enter New Blood Pressure (e.g. 70 for shock): "; cin >> bp;
                cout << "Enter New SpO2 (% e.g. 82 for severe hypoxia): "; cin >> o2;
                hospital.reTriagePatient(pid, hr, bp, o2);
                hospital.displayQueue();
                break;
            }

            case 6:
                hospital.saveReportToFile("triage_report.txt");
                break;

            case 7:
                cout << "\n======================================================================\n";
                cout << "                     FUTURE SCOPE & NEXT STEPS                        \n";
                cout << "======================================================================\n";
                cout << " 1. Doctor Assignment Module: Match doctors based on specialty (Cardiologist, Surgeon).\n";
                cout << " 2. Bed Management: Track available beds in ICU and Resuscitation Bays.\n";
                cout << " 3. Diagnostic Orders: Automatically order X-Rays, ECGs, and Blood Tests.\n";
                cout << " 4. Billing & Discharge: Print formal hospital bill and discharge summary.\n";
                cout << "======================================================================\n";
                break;

            default:
                cout << "\n[!] Invalid choice. Please choose 0 to 7.\n";
                break;
        }
    }

    return 0;
}
