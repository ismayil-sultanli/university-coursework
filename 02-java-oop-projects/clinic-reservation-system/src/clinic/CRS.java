package clinic;

import java.util.*;

public class CRS {
    private List<Hospital> hospitals = new ArrayList<>();
    private List<Doctor> doctors = new ArrayList<>();
    private List<Patient> patients = new ArrayList<>();
    private List<Appointment> appointments = new ArrayList<>();

    public void addHospital(String hospitalId, String hospitalName, String address) {
        if (hospitalExists(hospitalId, hospitalName)) {
            throw new IllegalArgumentException("Hospital with this ID or name already exists.");
        }
        Hospital hospital = new Hospital(hospitalId, hospitalName);
        hospitals.add(hospital);
    }

    private boolean hospitalExists(String hospitalId, String hospitalName) {
        for (Hospital hospital : hospitals) {
            if (hospital.getHospitalId().equals(hospitalId) || hospital.getHospitalName().equalsIgnoreCase(hospitalName)) {
                return true;
            }
        }
        return false;
    }

    public void addSection(String hospitalId, String sectionId, String sectionName) {
        Hospital hospital = findHospitalById(hospitalId);
        if (hospital != null) {
            if (sectionExistsInHospital(hospitalId, sectionId)) {
                throw new IllegalArgumentException("Section with this ID already exists in the hospital.");
            }
            Section section = new Section(sectionId, sectionName);
            hospital.addSection(section);
        }
    }

    private boolean sectionExistsInHospital(String hospitalId, String sectionId) {
        Hospital hospital = findHospitalById(hospitalId);
        if (hospital != null) {
            for (Section section : hospital.getSections()) {
                if (section.getSectionId().equals(sectionId)) {
                    return true;
                }
            }
        }
        return false;
    }

    public void addDoctor(String hospitalId, String sectionId, String doctorId, String doctorName, String doctorDiplomaId) {
        Hospital hospital = findHospitalById(hospitalId);
        if (hospital != null) {
            Section section = findSectionInHospital(hospitalId, sectionId);
            if (section != null) {
                if (doctorExists(doctorId)) {
                    throw new IllegalArgumentException("Doctor with this ID already exists.");
                }
                Doctor doctor = new Doctor(doctorId, doctorName, doctorDiplomaId);
                section.addDoctor(doctor);
                doctors.add(doctor);
            }
        }
    }

    private boolean doctorExists(String doctorId) {
        for (Doctor doctor : doctors) {
            if (doctor.getDoctorId().equals(doctorId)) {
                return true;
            }
        }
        return false;
    }

    public void addPatient(String patientId, String patientName) {
        if (patientExists(patientId)) {
            throw new IllegalArgumentException("Patient with this ID already exists.");
        }
        Patient patient = new Patient(patientId, patientName);
        patients.add(patient);
    }

    private boolean patientExists(String patientId) {
        for (Patient patient : patients) {
            if (patient.getPatientId().equals(patientId)) {
                return true;
            }
        }
        return false;
    }

    public void addAppointment(String hospitalId, String sectionId, String doctorId, String patientId, Date date) {
        Appointment appointment = new Appointment(hospitalId, sectionId, doctorId, patientId, date);
        appointments.add(appointment);
    }

    private Hospital findHospitalById(String hospitalId) {
        for (Hospital hospital : hospitals) {
            if (hospital.getHospitalId().equals(hospitalId)) {
                return hospital;
            }
        }
        return null;
    }

    private Section findSectionInHospital(String hospitalId, String sectionId) {
        Hospital hospital = findHospitalById(hospitalId);
        if (hospital != null) {
            for (Section section : hospital.getSections()) {
                if (section.getSectionId().equals(sectionId)) {
                    return section;
                }
            }
        }
        return null;
    }

    public List<Hospital> getHospitals() {
        return hospitals;
    }

    public List<Doctor> getDoctors() {
        return doctors;
    }

    public List<Patient> getPatients() {
        return patients;
    }

    public List<Appointment> getAppointments() {
        return appointments;
    }
}
