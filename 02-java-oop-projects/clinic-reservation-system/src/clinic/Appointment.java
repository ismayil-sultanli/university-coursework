package clinic;

import java.util.Date;

public class Appointment {
    private String hospitalId;
    private String sectionId;
    private String doctorId;
    private String patientId;
    private Date date;

    public Appointment(String hospitalId, String sectionId, String doctorId, String patientId, Date date) {
        this.hospitalId = hospitalId;
        this.sectionId = sectionId;
        this.doctorId = doctorId;
        this.patientId = patientId;
        this.date = date;
    }

    public String getHospitalId() {
        return hospitalId;
    }

    public String getSectionId() {
        return sectionId;
    }

    public String getDoctorId() {
        return doctorId;
    }

    public String getPatientId() {
        return patientId;
    }

    public Date getDate() {
        return date;
    }
}
