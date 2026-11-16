package clinic;

import java.util.LinkedList;
import java.util.Date;

public class Schedule {
    private LinkedList<Rendezvous> sessions;
    private int maxPatientsPerDay;

    public Schedule(int maxPatientsPerDay) {
        this.sessions = new LinkedList<>();
        this.maxPatientsPerDay = maxPatientsPerDay;
    }

    public boolean addRendezvous(Patient patient, Date desiredDate) {
        if (sessions.size() < maxPatientsPerDay) {
            Rendezvous newRendezvous = new Rendezvous(patient, desiredDate);
            sessions.add(newRendezvous);
            return true;
        }
        return false; 
    }

    public LinkedList<Rendezvous> getSessions() {
        return sessions;
    }

    public int getMaxPatientsPerDay() {
        return maxPatientsPerDay;
    }

    public void setMaxPatientsPerDay(int maxPatientsPerDay) {
        this.maxPatientsPerDay = maxPatientsPerDay;
    }
}
