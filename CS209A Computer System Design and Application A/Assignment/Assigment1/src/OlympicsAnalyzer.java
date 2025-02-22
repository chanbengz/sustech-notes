import java.io.FileReader;
import java.nio.file.Paths;
import java.util.*;
import java.nio.file.Path;
import java.io.IOException;
import java.nio.file.Files;
import java.io.BufferedReader;
import java.util.function.ToDoubleBiFunction;
import java.text.DecimalFormat;

public class OlympicsAnalyzer implements OlympicsAnalyzerInterface {
    Map<Integer, Athlete> athletes = new HashMap<>();
    Map<String, HashSet<Integer>> sport_to_years = new HashMap<>();
    Map<String, Country> countries = new HashMap<>();

    public static String[] stringSplit(String s) {
        List<String> res = new ArrayList<>();
        StringBuilder currentField = new StringBuilder();
        boolean inQuotes = false;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '"') {
                inQuotes = !inQuotes;
                currentField.append(c);
            } else if (c == ',' && !inQuotes) {
                res.add(currentField.toString());
                currentField.setLength(0);
            } else {
                currentField.append(c);
            }
        }
        res.add(currentField.toString());
        return res.toArray(new String[0]);
    }


    public static List<String[]> readCSVFile(Path filePath) throws IOException {
        List<String[]> records = new ArrayList<>();
        String line = "";
        try (BufferedReader br = new BufferedReader(new FileReader(filePath.toString()))) {
            br.readLine();
            while ((line = br.readLine()) != null) {
                String[] values = stringSplit(line);
                records.add(values);
            }
        }
        return records;
    }
    public OlympicsAnalyzer(String datasetPath) {
        // Load Olympic_Athlete_Bio_filtered.csv
        try {
            Path bioTable_path = Paths.get(datasetPath, "Olympic_Athlete_Bio_filtered.csv");
            List<String[]> Data = readCSVFile(bioTable_path);
            String name = "", height = "", weight = "", sex = "", birthdate = "", noc = "";
            int id, cnt = 0;
            for (String[] row : Data) {
                int t = 0;
                id = Integer.parseInt(row[t++]);
                name = row[t++];
                sex = row[t++];
                birthdate = row[t++];
                height = row[t++];
                weight = row[t++];
                noc = row[++t];
                Athlete athlete = new Athlete(id, name, sex, birthdate, height, weight, noc);
                athletes.put(id, athlete);
                //System.out.println(athlete);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        // Load Olympics_Country.csv
        try {
            Path contry_table_path = Paths.get(datasetPath, "Olympics_Country.csv");
            List<String[]> Data = readCSVFile(contry_table_path);
            String name = "", noc = "";
            for (String[] row : Data) {
                int t = 0;
                noc = row[t++];
                name = row[t++];
                Country country = new Country(name, noc);
                countries.put(noc, country);
                //System.out.println("name: " + name + " noc: " + noc);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        //for (String keys : countries.keySet()) {
        //    System.out.println(keys);
        //}
        // Load Olympic_Games_Medal_Tally.csv
        try {
            Path medel_count_path = Paths.get(datasetPath, "Olympic_Games_Medal_Tally.csv");
            List<String[]> Data = readCSVFile(medel_count_path);
            String edition = "", country = "", noc = "";
            int year_baseline = 2000;
            int edition_id, year, gold, silver, bronze, total;
            for (String[] row : Data) {
                int t = 0;
                edition = row[t++];
                edition_id = Integer.parseInt(row[t++]);
                year = Integer.parseInt(row[t++]);
                country = row[t++];
                noc = row[t++];
                gold = Integer.parseInt(row[t++]);
                silver = Integer.parseInt(row[t++]);
                bronze = Integer.parseInt(row[t++]);
                total = Integer.parseInt(row[t++]);
                //System.out.println("edition: " + edition + " year: " + year + " noc: " + noc + " total: " + total);
                if (edition.contains("Winter") && year >= year_baseline && countries.containsKey(noc)) {
                    countries.get(noc).addMedal(total);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        // Load Olympic_Athlete_Event_Results.csv
        try {
            Path results_path = Paths.get(datasetPath, "Olympic_Athlete_Event_Results.csv");
            List<String[]> Data = readCSVFile(results_path);
            String edition = "", athlete = "", noc = "", event = "", sport = "", medal = "", is_team;
            int result_id, edition_id, athlete_id, pos;
            for (String[] row : Data) {
                int t = 0;
                edition = row[t++];
                edition_id = Integer.parseInt(row[t++]);
                noc = row[t++];
                sport = row[t++];
                event = row[t++];
                result_id = Integer.parseInt(row[t++]);
                athlete = row[t++];
                athlete_id = Integer.parseInt(row[t++]);
                t = t + 1;
                medal = row[t++];
                is_team = row[t++];
                if (athletes.containsKey(athlete_id)) {
                    if (is_team.equals("False") && medal.equals("Gold")) {
                        athletes.get(athlete_id).addMedal(1);
                    }
                    athletes.get(athlete_id).addSport(sport);
                }
                if (edition.contains("2020 Summer Olympics")) {
                    if (athletes.get(athlete_id) == null) {
                        continue;
                    }
                    countries.get(noc).addAthlete(athletes.get(athlete_id));
                }
                if (edition.contains("Summer")) {
                    int year = Integer.parseInt(edition.split(" ")[0]);
                    if (!sport_to_years.containsKey(sport)) {
                        sport_to_years.put(sport, new HashSet<>());
                    }
                    sport_to_years.get(sport).add(year);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    @Override
    public Map<String, Integer> topPerformantFemale() {
        // Top 10 Performant Female Athletes in Individual Sport This method returns a <name, count> map of size 10
        // The map should be sorted by the descending order of count (i.e., from most to the least). If two athletes
        // have the same gold medal count, then they should be sorted by the alphabetical order of the name
        Map<String, Integer> femaleAthletes = new HashMap<>();
        for (Athlete athlete : athletes.values()) {
            if (athlete.getIsFemale()) {
                femaleAthletes.put(athlete.getName(), athlete.getMedal());
            }
        }
        List<Map.Entry<String, Integer>> list = new ArrayList<>(femaleAthletes.entrySet());
        list.sort((o1, o2) -> {
            int compare = o2.getValue().compareTo(o1.getValue());
            if (compare != 0) {
                return compare;
            } else {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        Map<String, Integer> top10FemaleAthletes = new LinkedHashMap<>();
        for (int i = 0; i < 10 && i < list.size(); i++) {
            top10FemaleAthletes.put(list.get(i).getKey(), list.get(i).getValue());
            //System.out.println(list.get(i).getKey() + " " + list.get(i).getValue());
        }
        return top10FemaleAthletes;
    }

    @Override
    public Map<String, Float> bmiBySports() {
        Map<String, Float> totalBmiBySport = new HashMap<>();
        Map<String, Integer> athleteCountBySport = new HashMap<>();
        for (Athlete athlete : athletes.values()) {
            for (String sport : athlete.getSports()) {
                if (athlete.getBmi() != -1) {
                    totalBmiBySport.put(sport, totalBmiBySport.getOrDefault(sport, 0.0f) + athlete.getBmi());
                    athleteCountBySport.put(sport, athleteCountBySport.getOrDefault(sport, 0) + 1);
                }
            }
        }
        DecimalFormat df = new DecimalFormat("#.#");
        totalBmiBySport.replaceAll((k, v) -> Float.valueOf(df.format(totalBmiBySport.get(k) / athleteCountBySport.get(k))));
        List<Map.Entry<String, Float>> list = new ArrayList<>(totalBmiBySport.entrySet());
        list.sort((o1, o2) -> {
            int compare = o2.getValue().compareTo(o1.getValue());
            if (compare != 0) {
                return compare;
            } else {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        Map<String, Float> bmiBySport = new LinkedHashMap<>();
        for (Map.Entry<String, Float> entry : list) {
            bmiBySport.put(entry.getKey(), entry.getValue());
            //System.out.println(entry.getKey() + " " + entry.getValue());
        }
        return bmiBySport;
    }

    @Override
    public Map<String, Set<Integer>> leastAppearedSport() {
        Map<String, Integer> sportCount = new HashMap<>();
        for (String sport : sport_to_years.keySet()) {
            sportCount.put(sport, sport_to_years.get(sport).size());
        }
        List<Map.Entry<String, Integer>> list = new ArrayList<>(sportCount.entrySet());
        list.sort((o1, o2) -> {
            int compare = o1.getValue().compareTo(o2.getValue());
            if (compare != 0) {
                return compare;
            } else {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        Map<String, Set<Integer>> leastAppearedSport = new LinkedHashMap<>();
        for (int i = 0; i < 10 && i < list.size(); i++) {
            leastAppearedSport.put(list.get(i).getKey(), sport_to_years.get(list.get(i).getKey()));
            //System.out.println(list.get(i).getKey() + " " + list.get(i).getValue());
        }
        return leastAppearedSport;
    }

    @Override
    public Map<String, Integer> winterMedalsByCountry() {
        Map<String, Integer> winterMedalsByCountry = new HashMap<>();
        for (Country country : countries.values()) {
            winterMedalsByCountry.put(country.getNoc(), country.getMedals());
        }
        List<Map.Entry<String, Integer>> list = new ArrayList<>(winterMedalsByCountry.entrySet());
        list.sort((o1, o2) -> {
            int compare = o2.getValue().compareTo(o1.getValue());
            if (compare != 0) {
                return compare;
            } else {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        Map<String, Integer> top10WinterMedalsByCountry = new LinkedHashMap<>();
        for (int i = 0; i < 10 && i < list.size(); i++) {
            top10WinterMedalsByCountry.put(list.get(i).getKey(), list.get(i).getValue());
            System.out.println(list.get(i).getKey() + " " + list.get(i).getValue());
        }
        return top10WinterMedalsByCountry;
    }

    @Override
    public Map<String, Integer> topCountryWithYoungAthletes() {
        Map<String, Integer> AverageAgeByCountry = new HashMap<>();
        for (Country country : countries.values()) {
            int totalAge = 0, athleteCount = 0;
            if (country.getAthletesSize() == 0) {
                continue;
            }
            for (Athlete athlete : country.getAthletes()) {
                //if (country.getName().equals("Bangladesh")) {
                //    System.out.println("Special: " + athlete.getName() + " " + athlete.getBornYear());
                //}
                if (athlete.getBornYear() != -1) {
                    totalAge += 2020 - athlete.getBornYear();
                    athleteCount++;
                }
            }
            AverageAgeByCountry.put(country.getName(), Math.round((float)totalAge / athleteCount));
        }
        List<Map.Entry<String, Integer>> list = new ArrayList<>(AverageAgeByCountry.entrySet());
        list.sort((o1, o2) -> {
            int compare = o1.getValue().compareTo(o2.getValue());
            if (compare != 0) {
                return compare;
            } else {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        Map<String, Integer> topCountryWithYoungAthletes = new LinkedHashMap<>();
        for (int i = 0; i < 10 && i < list.size(); i++) {
            topCountryWithYoungAthletes.put(list.get(i).getKey(), list.get(i).getValue());
            //System.out.println(list.get(i).getKey() + " " + list.get(i).getValue());
        }
        return topCountryWithYoungAthletes;
    }
}
