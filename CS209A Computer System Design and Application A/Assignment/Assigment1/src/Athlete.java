import java.util.HashSet;
import java.util.Set;
public class Athlete {
    private int id, born_year, medal;
    private float bmi;
    private String name, noc;
    private boolean isFemale;

    private Set<String> sports;

    public Athlete(int id, String name, String sex, String birthdate, String height, String weight, String noc) {
        this.id = id;
        this.name = name;
        this.medal = 0;
        if (sex.equals("Female")) {
            this.isFemale = true;
        } else {
            this.isFemale = false;
        }
        if (birthdate.isEmpty()) {
            this.born_year = -1;
        } else {
            String[] tmp = birthdate.split(" ");
            int len = tmp.length;
            this.born_year = Integer.parseInt(tmp[len - 1]);
        }
        if (height.isEmpty() || weight.isEmpty()) {
            this.bmi = -1;
        } else {
            this.bmi = Float.parseFloat(weight) / (Float.parseFloat(height) * Float.parseFloat(height) / 10000.0f);
        }
        this.noc = noc;
        this.sports = new HashSet<>();
    }
    public void addSport(String sport) {
        this.sports.add(sport);
    }
    public void addMedal(int k) {
        this.medal += k;
    }
    public int getId() {
        return this.id;
    }
    public String getName() {
        return this.name;
    }
    public boolean getIsFemale() {
        return this.isFemale;
    }
    public int getBornYear() {
        return this.born_year;
    }
    public float getBmi() {
        return this.bmi;
    }
    public String getNoc() {
        return this.noc;
    }
    public Set<String> getSports() {
        return this.sports;
    }
    public int getMedal() {
        return this.medal;
    }

    public String toString() {
        return "Athlete{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", born_year=" + born_year +
                ", medal=" + medal +
                ", bmi=" + bmi +
                ", isFemale=" + isFemale +
                ", noc='" + noc + '\'' +
                ", sports=" + sports +
                '}';
    }
}
