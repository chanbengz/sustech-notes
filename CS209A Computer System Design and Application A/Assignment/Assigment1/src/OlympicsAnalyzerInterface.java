import java.util.Map;
import java.util.Set;

public interface OlympicsAnalyzerInterface {
    Map<String, Integer> topPerformantFemale();
    Map<String, Float> bmiBySports();
    Map<String, Set<Integer>> leastAppearedSport();
    Map<String, Integer> winterMedalsByCountry();
    Map<String, Integer> topCountryWithYoungAthletes();
}
