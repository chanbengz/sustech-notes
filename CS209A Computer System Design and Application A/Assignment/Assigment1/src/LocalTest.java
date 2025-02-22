import org.junit.jupiter.api.Test;

import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

public class LocalTest {

    OlympicsAnalyzer olympicsAnalyzer = new OlympicsAnalyzer(Paths.get("src", "local_test_data").toString());

//    @Test
//    void printExpectedAthletesPutStatements() {
//        Map<String, Integer> expectedAthletes = olympicsAnalyzer.topPerformantFemale();
//        for (Map.Entry<String, Integer> entry : expectedAthletes.entrySet()) {
//            System.out.println("expectedAthletes.put(\"" + entry.getKey() + "\", " + entry.getValue() + ");");
//        }
//    }

    @Test
    void topPerformantFemale() {
        Map<String, Integer> result = olympicsAnalyzer.topPerformantFemale();

        // Verify the map is not empty
        assertFalse(result.isEmpty(), "The result map should not be empty");

        // Verify the map contains the expected top-performing female athletes in the correct order
        Map<String, Integer> expectedAthletes = new LinkedHashMap<>();
        expectedAthletes.put("Janica Kostelić", 3);
        expectedAthletes.put("Katie Ledecky", 3);
        expectedAthletes.put("Elena Kaliská", 2);
        expectedAthletes.put("Fu Mingxia", 2);
        expectedAthletes.put("Irene Schouten", 2);
        expectedAthletes.put("Jin Seon-Yu", 2);
        expectedAthletes.put("Karin Janz", 2);
        expectedAthletes.put("Katja Seizinger", 2);
        expectedAthletes.put("Paola Pezzo", 2);
        expectedAthletes.put("Pat McCormick", 2);

        assertEquals(expectedAthletes.size(), result.size(), "The number of top-performing female athletes should match the expected number");

        Iterator<Map.Entry<String, Integer>> expectedIterator = expectedAthletes.entrySet().iterator();
        Iterator<Map.Entry<String, Integer>> resultIterator = result.entrySet().iterator();

        while (expectedIterator.hasNext() && resultIterator.hasNext()) {
            Map.Entry<String, Integer> expectedEntry = expectedIterator.next();
            Map.Entry<String, Integer> resultEntry = resultIterator.next();

            assertEquals(expectedEntry.getKey(), resultEntry.getKey(), "The athlete name should match");
            assertEquals(expectedEntry.getValue(), resultEntry.getValue(), "The count for athlete " + expectedEntry.getKey() + " should match the expected count");
        }
    }



//    @Test
//    void printExpectedSportsPutStatements() {
//        Map<String, Float> expectedSports = olympicsAnalyzer.bmiBySports();
//        for (Map.Entry<String, Float> entry : expectedSports.entrySet()) {
//            System.out.println("expectedSports.put(\"" + entry.getKey() + "\", " + entry.getValue() + "f);");
//        }
//    }


    @Test
    void bmiBySports() {
        Map<String, Float> result = olympicsAnalyzer.bmiBySports();

        // Verify the map is not empty
        assertFalse(result.isEmpty(), "The result map should not be empty");

        // Verify the map contains the expected sports and their average BMI values
        Map<String, Float> expectedSports = new LinkedHashMap<>();
        expectedSports.put("Tug-Of-War", 28.6f);
        expectedSports.put("Weightlifting", 27.4f);
        expectedSports.put("Bobsleigh", 26.7f);
        expectedSports.put("Australian Rules Football", 25.8f);
        expectedSports.put("Rugby", 25.4f);
        expectedSports.put("Baseball", 25.2f);
        expectedSports.put("Judo", 25.2f);
        expectedSports.put("Cycling BMX Racing", 25.0f);
        expectedSports.put("Ice Hockey", 25.0f);
        expectedSports.put("Rugby Sevens", 24.9f);
        expectedSports.put("Skeleton", 24.8f);
        expectedSports.put("Water Polo", 24.7f);
        expectedSports.put("Wrestling", 24.5f);
        expectedSports.put("Art Competitions", 24.4f);
        expectedSports.put("Shooting", 24.4f);
        expectedSports.put("Luge", 24.3f);
        expectedSports.put("Skateboarding", 24.3f);
        expectedSports.put("Canoe Sprint", 24.1f);
        expectedSports.put("Golf", 23.9f);
        expectedSports.put("Handball", 23.9f);
        expectedSports.put("Softball", 23.8f);
        expectedSports.put("Canoe Marathon", 23.7f);
        expectedSports.put("Alpine Skiing", 23.6f);
        expectedSports.put("Curling", 23.5f);
        expectedSports.put("Sailing", 23.5f);
        expectedSports.put("Archery", 23.4f);
        expectedSports.put("Rowing", 23.4f);
        expectedSports.put("Basketball", 23.2f);
        expectedSports.put("Cycling Track", 23.2f);
        expectedSports.put("Freestyle Skiing", 22.9f);
        expectedSports.put("Speed Skating", 22.9f);
        expectedSports.put("Hockey", 22.8f);
        expectedSports.put("Surfing", 22.8f);
        expectedSports.put("Beach Volleyball", 22.7f);
        expectedSports.put("Football", 22.7f);
        expectedSports.put("Snowboarding", 22.7f);
        expectedSports.put("Bowling", 22.6f);
        expectedSports.put("Canoe Slalom", 22.5f);
        expectedSports.put("Equestrian Jumping", 22.5f);
        expectedSports.put("Fencing", 22.5f);
        expectedSports.put("Volleyball", 22.3f);
        expectedSports.put("Equestrian Eventing", 22.2f);
        expectedSports.put("Athletics", 22.1f);
        expectedSports.put("Badminton", 22.1f);
        expectedSports.put("Modern Pentathlon", 22.1f);
        expectedSports.put("Table Tennis", 22.1f);
        expectedSports.put("Tennis", 22.1f);
        expectedSports.put("Biathlon", 22.0f);
        expectedSports.put("Cross Country Skiing", 21.9f);
        expectedSports.put("Cycling Road", 21.9f);
        expectedSports.put("Swimming", 21.9f);
        expectedSports.put("Marathon Swimming", 21.8f);
        expectedSports.put("Short Track Speed Skating", 21.8f);
        expectedSports.put("Diving", 21.7f);
        expectedSports.put("Boxing", 21.6f);
        expectedSports.put("Equestrian Dressage", 21.6f);
        expectedSports.put("Wushu", 21.6f);
        expectedSports.put("Taekwondo", 21.5f);
        expectedSports.put("3x3 Basketball", 21.4f);
        expectedSports.put("Trampolining", 21.4f);
        expectedSports.put("Lacrosse", 21.3f);
        expectedSports.put("Nordic Combined", 21.3f);
        expectedSports.put("Artistic Gymnastics", 21.0f);
        expectedSports.put("Cycling Mountain Bike", 20.8f);
        expectedSports.put("Ski Jumping", 20.8f);
        expectedSports.put("Figure Skating", 20.7f);
        expectedSports.put("Triathlon", 20.4f);
        expectedSports.put("Speed Skiing", 19.6f);
        expectedSports.put("Artistic Swimming", 19.5f);
        expectedSports.put("Karate", 18.8f);
        expectedSports.put("Rhythmic Gymnastics", 17.2f);

        assertEquals(expectedSports.size(), result.size(), "The number of sports should match the expected number");

        Iterator<Map.Entry<String, Float>> expectedIterator = expectedSports.entrySet().iterator();
        Iterator<Map.Entry<String, Float>> resultIterator = result.entrySet().iterator();

        while (expectedIterator.hasNext() && resultIterator.hasNext()) {
            Map.Entry<String, Float> expectedEntry = expectedIterator.next();
            Map.Entry<String, Float> resultEntry = resultIterator.next();

            assertEquals(expectedEntry.getKey(), resultEntry.getKey(), "The sport name should match");
            assertEquals(expectedEntry.getValue(), resultEntry.getValue(), "The average BMI for sport " + expectedEntry.getKey() + " should match the expected value");
        }
    }

//    @Test
//    void printExpectedSportsPutStatements() {
//        Map<String, Set<Integer>> expectedSports = olympicsAnalyzer.leastAppearedSport();
//        for (Map.Entry<String, Set<Integer>> entry : expectedSports.entrySet()) {
//            System.out.print("expectedSports.put(\"" + entry.getKey() + "\", new HashSet<>(Arrays.asList(");
//            System.out.print(entry.getValue().stream().map(String::valueOf).collect(Collectors.joining(", ")));
//            System.out.println(")));");
//        }
//    }

    @Test
    void leastAppearedSport() {
        Map<String, Set<Integer>> result = olympicsAnalyzer.leastAppearedSport();

        // Verify the map is not empty
        assertFalse(result.isEmpty(), "The result map should not be empty");

        // Verify the map contains the expected sports and their appearance years
        Map<String, Set<Integer>> expectedSports = new LinkedHashMap<>();
        expectedSports.put("3x3 Basketball", new HashSet<>(Arrays.asList(2020)));
        expectedSports.put("Alpinism", new HashSet<>(Arrays.asList(1936)));
        expectedSports.put("Australian Rules Football", new HashSet<>(Arrays.asList(1956)));
        expectedSports.put("Automobile Racing", new HashSet<>(Arrays.asList(1900)));
        expectedSports.put("Ballooning", new HashSet<>(Arrays.asList(1900)));
        expectedSports.put("Bicycle Polo", new HashSet<>(Arrays.asList(1908)));
        expectedSports.put("Boules", new HashSet<>(Arrays.asList(1900)));
        expectedSports.put("Bowling", new HashSet<>(Arrays.asList(1988)));
        expectedSports.put("Canne De Combat", new HashSet<>(Arrays.asList(1924)));
        expectedSports.put("Cricket", new HashSet<>(Arrays.asList(1900)));

        assertEquals(expectedSports.size(), result.size(), "The number of sports should match the expected number");

        Iterator<Map.Entry<String, Set<Integer>>> expectedIterator = expectedSports.entrySet().iterator();
        Iterator<Map.Entry<String, Set<Integer>>> resultIterator = result.entrySet().iterator();

        while (expectedIterator.hasNext() && resultIterator.hasNext()) {
            Map.Entry<String, Set<Integer>> expectedEntry = expectedIterator.next();
            Map.Entry<String, Set<Integer>> resultEntry = resultIterator.next();

            assertEquals(expectedEntry.getKey(), resultEntry.getKey(), "The sport name should match");
            assertEquals(new ArrayList<>(expectedEntry.getValue()).stream().sorted().collect(Collectors.toList()),
                    new ArrayList<>(resultEntry.getValue()).stream().sorted().collect(Collectors.toList()),
                    "The appearance years for sport " + expectedEntry.getKey() + " should match the expected years");
        }

        // Verify the map is sorted by the ascending order of the number of appearances and then by the alphabetical order of the sport name
        Integer previousSize = null;
        String previousName = null;
        for (Map.Entry<String, Set<Integer>> entry : result.entrySet()) {
            if (previousSize != null) {
                assertTrue(previousSize < entry.getValue().size() || (previousSize.equals(entry.getValue().size()) && previousName.compareTo(entry.getKey()) <= 0),
                        "The map is not sorted by ascending order of the number of appearances and then by the alphabetical order of the sport name");
            }
            previousSize = entry.getValue().size();
            previousName = entry.getKey();
        }
    }


//    @Test
//    void printExpectedCountriesPutStatements() {
//        Map<String, Integer> expectedCountries = olympicsAnalyzer.winterMedalsByCountry();
//        for (Map.Entry<String, Integer> entry : expectedCountries.entrySet()) {
//            System.out.println("expectedCountries.put(\"" + entry.getKey() + "\", " + entry.getValue() + ");");
//        }
//    }

    @Test
    void winterMedalsByCountry() {
        Map<String, Integer> result = olympicsAnalyzer.winterMedalsByCountry();

        // Verify the map is not empty
        assertFalse(result.isEmpty(), "The result map should not be empty");

        // Verify the map contains the expected countries and their medal counts
        Map<String, Integer> expectedCountries = new LinkedHashMap<>();
        expectedCountries.put("USA", 65);
        expectedCountries.put("FRA", 46);
        expectedCountries.put("KOR", 39);
        expectedCountries.put("SWE", 33);
        expectedCountries.put("AUT", 31);
        expectedCountries.put("JPN", 27);
        expectedCountries.put("CAN", 26);
        expectedCountries.put("CHN", 24);
        expectedCountries.put("NOR", 23);
        expectedCountries.put("ITA", 22);

        assertEquals(expectedCountries.size(), result.size(), "The number of countries should match the expected number");

        Iterator<Map.Entry<String, Integer>> expectedIterator = expectedCountries.entrySet().iterator();
        Iterator<Map.Entry<String, Integer>> resultIterator = result.entrySet().iterator();

        while (expectedIterator.hasNext() && resultIterator.hasNext()) {
            Map.Entry<String, Integer> expectedEntry = expectedIterator.next();
            Map.Entry<String, Integer> resultEntry = resultIterator.next();

            assertEquals(expectedEntry.getKey(), resultEntry.getKey(), "The country name should match");
            assertEquals(expectedEntry.getValue(), resultEntry.getValue(), "The medal count for country " + expectedEntry.getKey() + " should match the expected value");
        }

        // Verify the map is sorted by the descending order of medal counts and then by the alphabetical order of the country name
        Integer previousCount = null;
        String previousName = null;
        for (Map.Entry<String, Integer> entry : result.entrySet()) {
            if (previousCount != null) {
                assertTrue(previousCount > entry.getValue() || (previousCount.equals(entry.getValue()) && previousName.compareTo(entry.getKey()) <= 0),
                        "The map is not sorted by descending order of medal counts and then by the alphabetical order of the country name");
            }
            previousCount = entry.getValue();
            previousName = entry.getKey();
        }
    }


//    @Test
//    void printExpectedCountriesPutStatements() {
//        Map<String, Integer> expectedCountries = olympicsAnalyzer.topCountryWithYoungAthletes();
//        for (Map.Entry<String, Integer> entry : expectedCountries.entrySet()) {
//            System.out.println("expectedCountries.put(\"" + entry.getKey() + "\", " + entry.getValue() + ");");
//        }
//    }

    @Test
    void topCountryWithYoungAthletes() {
        Map<String, Integer> result = olympicsAnalyzer.topCountryWithYoungAthletes();

        // Verify the map is not empty
        assertFalse(result.isEmpty(), "The result map should not be empty");

        // Verify the map contains the expected countries and their average ages
        Map<String, Integer> expectedCountries = new LinkedHashMap<>();
        expectedCountries.put("Cayman Islands", 14);
        expectedCountries.put("Ghana", 16);
        expectedCountries.put("Kosovo", 16);
        expectedCountries.put("Namibia", 17);
        expectedCountries.put("Armenia", 18);
        expectedCountries.put("Bangladesh", 18);
        expectedCountries.put("Costa Rica", 18);
        expectedCountries.put("Federated States of Micronesia", 18);
        expectedCountries.put("Nepal", 18);
        expectedCountries.put("Turkmenistan", 18);

        assertEquals(expectedCountries.size(), result.size(), "The number of countries should match the expected number");

        Iterator<Map.Entry<String, Integer>> expectedIterator = expectedCountries.entrySet().iterator();
        Iterator<Map.Entry<String, Integer>> resultIterator = result.entrySet().iterator();

        while (expectedIterator.hasNext() && resultIterator.hasNext()) {
            Map.Entry<String, Integer> expectedEntry = expectedIterator.next();
            Map.Entry<String, Integer> resultEntry = resultIterator.next();

            assertEquals(expectedEntry.getKey(), resultEntry.getKey(), "The country name should match");
            assertEquals(expectedEntry.getValue(), resultEntry.getValue(), "The average age for country " + expectedEntry.getKey() + " should match the expected value");
        }

        // Verify the map is sorted by the ascending order of average ages and then by the alphabetical order of the country name
        Integer previousAge = null;
        String previousName = null;
        for (Map.Entry<String, Integer> entry : result.entrySet()) {
            if (previousAge != null) {
                assertTrue(previousAge < entry.getValue() || (previousAge.equals(entry.getValue()) && previousName.compareTo(entry.getKey()) <= 0),
                        "The map is not sorted by ascending order of average ages and then by the alphabetical order of the country name");
            }
            previousAge = entry.getValue();
            previousName = entry.getKey();
        }
    }
}