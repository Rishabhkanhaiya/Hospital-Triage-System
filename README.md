# 🏥 Hospital Emergency Triage System

An automated Emergency Room (ER) patient prioritization and acuity management system developed in **C++** implementing core **Object-Oriented Programming (OOPS)** principles.

Developed as a Course Project by 2nd Year B.Tech students in the **Department of Computer Science Engineering (Artificial Intelligence)** at **Vishwakarma Institute of Technology, Pune**.

---

## 👥 Project Team Members
1. **Rishabh Joshi** (1251070307)
2. **Ujjwal Jain** (1251071062)
3. **Shubham Kale** (1251071039)
4. **Harshad Mahadik** (1251071041)

---

## 📌 Problem Statement & Clinical Motivation

In traditional public service systems (banks, railway ticket counters), queues follow **First-In, First-Out (FIFO)** scheduling. However, in a **Hospital Emergency Room (ER)**, FIFO is dangerous and fatal:
* A patient arriving at 10:00 AM with a minor ankle sprain cannot take precedence over a patient arriving at 10:05 AM suffering an acute myocardial infarction (heart attack).
* Medical outcomes decline drastically if critical resuscitation cases face triage delays beyond the clinical **"Golden Hour"**.
* Manual paper triage is subjective, prone to nursing cognitive fatigue, and fails when a patient's vital signs deteriorate silently while waiting in the hall.

**The Solution:**
Our system automates physiological vitals assessment, classifies patients into standard clinical acuity tiers, maintains a priority queue using operator overloading, and dynamically escalates deteriorating patients in real time.

---

## 🚦 Clinical Urgency Tiers

The system scores patients based on the global **Emergency Severity Index (ESI v4)** criteria:

| Priority Level | Clinical Tier | Color Tag | Criteria & Physiological Triggers | Target Response |
|:---:|:---:|:---:|---|:---:|
| **Level 1** | **CRITICAL** | 🔴 `[RED]` | $\text{SpO}_2 < 88\%$, $\text{Heart Rate} > 130$ or $< 45\text{ bpm}$, $\text{Systolic BP} < 80\text{ mmHg}$, or severe trauma hemorrhage. | **0 mins (Immediate)** |
| **Level 2** | **URGENT** | 🟡 `[YELLOW]` | $\text{SpO}_2 < 94\%$, $\text{Heart Rate} > 100\text{ bpm}$, $\text{Systolic BP} > 160\text{ mmHg}$ or $< 90\text{ mmHg}$, high fever with respiratory distress. | **$<$ 15 mins** |
| **Level 3** | **NORMAL** | 🟢 `[GREEN]` | Stable baseline vitals, minor lacerations, localized sprains, clinic level. | **$<$ 60 mins** |

---

## 🧬 OOPS Concepts Demonstrated in C++

| OOP Concept | Implementation in Code | Description |
|---|---|---|
| **Encapsulation & Data Hiding** | `class Vitals` | Biological variables (`heartRate`, `systolicBP`, `spo2`) are private. Public setters enforce medical invariants (e.g. $\text{SpO}_2 \in [0, 100]\%$) and throw `invalid_argument` exceptions on violation. |
| **Inheritance** | `class Person` $\rightarrow$ `class Patient` | Base class `Person` encapsulates identity (`id`, `name`, `age`, `phone`). Derived class `Patient` inherits publicly and adds emergency complaints, vitals, and queue timestamps. |
| **Hierarchical Inheritance** | `class Patient` $\rightarrow$ `CardiacPatient`, `TraumaPatient` | Specialized patient classes add sub-domain parameters (`severeChestPain`, `ecgStatus`, `injuryType`, `activeBleeding`). |
| **Runtime Polymorphism** | `virtual void displayDetails() const override` | Virtual functions and destructors (`virtual ~Person()`) resolved at runtime through vtables for specialized clinical cards. |
| **Compile-Time Polymorphism** | `bool operator<(const Patient& other) const` | Overloaded `<` operator to rank patients in the waiting queue by urgency priority first, with FIFO arrival tie-breaking. |
| **Exception Handling** | `try`, `throw`, `catch` | Defensive boundary checking catches invalid input parameters without crashing the program. |
| **File Handling / Streams** | `std::ofstream` | Serializes patient intake and queue snapshots to persistent audit files (`triage_report.txt`). |

---

## 🏗️ System Architecture & Class Hierarchy

```
                       ┌─────────────────────────┐
                       │       class Vitals      │  (Encapsulation)
                       └────────────┬────────────┘
                                    │ has-a (Composition)
 ┌──────────────────────┐           │
 │     class Person     │           │  (Base Entity)
 └──────────┬───────────┘           │
            │ is-a (Inheritance)    │
 ┌──────────▼───────────────────────▼────────────┐
 │                class Patient                  │  (Operator Overloading <)
 └──────────┬───────────────────────┬────────────┘
            │ is-a                  │ is-a
 ┌──────────▼───────────┐ ┌─────────▼────────────┐
 │ class CardiacPatient │ │ class TraumaPatient  │  (Polymorphism)
 └──────────────────────┘ └──────────────────────┘
            │                       │
 ┌──────────▼───────────────────────▼────────────┐
 │             class HospitalSystem              │  (Controller Facade)
 └──────────────────────┬────────────────────────┘
                        │
 ┌──────────────────────▼────────────────────────┐
 │                   int main()                  │  (Interactive CLI)
 └───────────────────────────────────────────────┘
```

---

## ⚡ The Key Innovation: Dynamic Clinical Re-Triage

In standard queue systems, priority is static once assigned. In our system:
1. Patient **P101 (Siddharth Verma)** arrives with a mild ankle sprain $\rightarrow$ Assigned **Priority 3 (Normal Green)** at the bottom of the queue (**Rank #4**).
2. While waiting, the patient suffers acute shock and hypoxia $\rightarrow$ Vitals crash: $\text{HR} = 150\text{ bpm}$, $\text{BP} = 70/45\text{ mmHg}$, $\text{SpO}_2 = 82\%$.
3. The nurse enters new vitals via Option 5 (`reTriagePatient()`).
4. The system updates the vitals, recalculates the priority to **Priority 1 (Critical Red)**, re-sorts the queue, and **instantly promotes the patient from Rank #4 straight to Rank #1**!

---

## 🚀 How to Compile & Run

### Prerequisites
* A C++ compiler supporting C++11 or higher (e.g. `g++`, `clang++`, or MSVC).

### Option 1: 1-Click Batch Runner (Windows)
Double-click `run.bat` in the project root.

### Option 2: Terminal / Command Prompt
```bash
# Compile
g++ -Wall -Wextra simple_emergency_triage.cpp -o simple_triage.exe

# Run
./simple_triage.exe
```

---

## 🎮 Interactive Menu Options
```
---------------------------- MENU OPTIONS ----------------------------
 1. [DEMO] Load 4 Sample Patients (Fast Demonstration)
 2. Register New Patient (Manual Entry with Vitals Validation)
 3. Show Live Waiting Queue (Sorted by Priority)
 4. Admit / Call Next Patient to Doctor Bay
 5. Dynamic Re-Triage (Simulate Patient Condition Getting Worse)
 6. Save Triage Report to Text File (File Handling)
 7. Future Scope & Next Steps
 0. Exit System
----------------------------------------------------------------------
```

---

## 🔮 Future Scope
1. **Automated Doctor & Specialist Allocation**: Matching specialized patient classes (`CardiacPatient`, `TraumaPatient`) directly to on-duty specialists (Cardiologists, Trauma Surgeons) via an `EmergencyDoctor` class.
2. **Hospital Ward & ICU Bed Management**: Tracking real-time occupancy of Resuscitation Bays, ICU beds, and Observation units with automated bed reservations upon admission.
3. **Emergency Diagnostic Lab & Radiology Orders**: Automated ordering of emergency STAT tests (ECG, CT scans, X-rays, Blood Gas tests) directly from triage.
4. **Electronic Health Record (EHR) Discharge & Billing**: Generating computerized discharge summaries and itemized bills based on clinical resources consumed.
5. **IoT Wireless Patient Telemetry**: Connecting wireless pulse oximeter bands to stream live vitals and trigger automatic re-triage alerts without manual nurse data entry.

---

## 📜 License
This project is open-source and available under the [MIT License](LICENSE).
