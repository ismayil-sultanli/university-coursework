package clinic;

import java.util.Date;

public class Rendezvous {
    private Patient patient;
    private Date dateTime;

    public Rendezvous(Patient patient, Date dateTime) {
        this.patient = patient;
        this.dateTime = dateTime;
    }

    public Patient getPatient() {
        return patient;
    }

    public Date getDateTime() {
        return dateTime;
    }

    @Override
    public String toString() {
        return "Rendezvous with patient: " + patient.getPatientName() + " on " + dateTime.toString();
    }
}
