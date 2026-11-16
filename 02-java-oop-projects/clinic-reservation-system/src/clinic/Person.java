package clinic;

public class Person {
    private String name;
    private long nationalId;

    public Person(String name, long nationalId) {
        this.name = name;
        this.nationalId = nationalId;
    }

    public String getName() {
        return name;
    }

    public long getNationalId() {
        return nationalId;
    }

    @Override
    public String toString() {
        return "Name: " + name + ", ID: " + nationalId;
    }
}
