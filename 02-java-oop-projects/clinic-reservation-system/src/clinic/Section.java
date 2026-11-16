package clinic;

import java.util.*;

public class Section {
    private String sectionId;
    private String sectionName;
    private List<Doctor> doctors = new ArrayList<>();

    public Section(String sectionId, String sectionName) {
        this.sectionId = sectionId;
        this.sectionName = sectionName;
    }

    public String getSectionId() {
        return sectionId;
    }

    public String getSectionName() {
        return sectionName;
    }

    public void addDoctor(Doctor doctor) {
        doctors.add(doctor);
    }

    public List<Doctor> getDoctors() {
        return doctors;
    }
}
