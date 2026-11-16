package clinic;

import java.util.*;

public class Hospital {
    private String hospitalId;
    private String hospitalName;
    private List<Section> sections = new ArrayList<>();

    public Hospital(String hospitalId, String hospitalName) {
        this.hospitalId = hospitalId;
        this.hospitalName = hospitalName;
    }

    public String getHospitalId() {
        return hospitalId;
    }

    public String getHospitalName() {
        return hospitalName;
    }

    public List<Section> getSections() {
        return sections;
    }

    public void addSection(Section section) {
        sections.add(section);
    }
}
