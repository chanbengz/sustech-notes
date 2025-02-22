import java.util.Set;
import java.util.HashSet;
public class Country {
    private String name, noc;
    int medals_for_winter;
    Set<Athlete> athletes_for_2020;

    public Country(String name, String noc) {
        this.name = name;
        this.noc = noc;
        this.medals_for_winter = 0;
        this.athletes_for_2020 = new HashSet<>();
    }

    public void addMedal(int k) {
        this.medals_for_winter += k;
    }

    public void addAthlete(Athlete athlete) {
        this.athletes_for_2020.add(athlete);
    }

    public String getName() {
        return this.name;
    }

    public String getNoc() {
        return this.noc;
    }

    public int getMedals() {
        return this.medals_for_winter;
    }

    public Set<Athlete> getAthletes() {
        return this.athletes_for_2020;
    }

    public int getAthletesSize() {
        return this.athletes_for_2020.size();
    }
}
