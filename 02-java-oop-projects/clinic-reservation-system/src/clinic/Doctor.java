package clinic;

public class Doctor {
    private String doctorId;
    private String doctorName;
    private String doctorDiplomaId;

    public Doctor(String doctorId, String doctorName, String doctorDiplomaId) {
        this.doctorId = doctorId;
        this.doctorName = doctorName;
        this.doctorDiplomaId = doctorDiplomaId;
    }

    public String getDoctorId() {
        return doctorId;
    }

    public String getDoctorName() {
        return doctorName;
    }

    public String getDoctorDiplomaId() {
        return doctorDiplomaId;
    }
}
