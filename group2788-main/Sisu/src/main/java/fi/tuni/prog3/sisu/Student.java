package fi.tuni.prog3.sisu;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonPrimitive;
import com.google.gson.reflect.TypeToken;
import java.io.*;
import java.lang.reflect.Type;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Class for storing and exchanging the state of the choices made by the user.
 * The data is stored and read via JSON-file.
 * Also loads course data structure via API HTTP-calls into a database 
 * structure. The data is too in JSON format and it's handled within the class.
 * @author Antti Nylund
 */
public class Student implements iReadAndWriteToFile, iAPI 
{
    private String name;
    private String degreeGroupId;
    private int studentNumber;
    private String selectedDegree;
    private String selectedProgram;
    private boolean programsChosen;
    private List<String> programs;
    private List<String> selectedCourses;
    private List<String> completedCourses;
    private HashMap<String, CourseUnit> courseDatabase;
    private int completedCredits;
    private int expectedGraduatingYear;
    private HashMap<String, Module> moduleData;
    private HashMap<String, HashMap<String, ArrayList<String>>> 
            modules = new HashMap<>();
    
    // Constructor
    public Student(String name, int studentNumber) 
    {
        this.name = name;
        this.studentNumber = studentNumber;      
        this.programs = new ArrayList<>();
        this.completedCourses = new ArrayList<>();
        this.selectedCourses = new ArrayList<>();
        this.selectedDegree = "";
        this.selectedProgram = "";
        this.courseDatabase = new HashMap<String, CourseUnit>();
        this.programsChosen = false;
    }

    // Default constructor
    public Student() 
    {
    }

    /**
     * Getter function for student's name.
     * @return Student's name
     */
    public String getName() 
    {
        return name;
    }

    /**
     * Setter function for student's name.
     * @param name - The name to be given for student.
     */
    public void setName(String name) 
    {
        this.name = name;
    }
    public void setDegreeGroupId(String degreeGroupId)
    {
        this.degreeGroupId = degreeGroupId;
    }
    public String getDegreeGroupId()
    {
        return this.degreeGroupId;
    }
    /**
     * Getter function for student's student number.
     * @return The student number associated with given student.
     */
    public int getStudentNumber() 
    {
        return studentNumber;
    }

    /**
     * Setter function for student's student number.
     * @param studentNumber - The student number to be associated with given 
     * student.
     */
    public void setStudentNumber(int studentNumber) 
    {
        this.studentNumber = studentNumber;
    }


    /**
     * Setter function for student's  selected degree. 
     * Also sets chosen student program to default, unselected state.
     * @param degree - the degree to be passed on as the student's choice.
     */
    public void setSelectedDegree(String degree) throws IOException 
    {
        this.selectedDegree = degree;
        this.selectedProgram = "";
        this.loadTree();
        setProgramsChosen(true);
        this.loadTree();
        
    }
    /**
     * getter function for student's selected degree.
     * @return The students selected degree
     */
    public String getSelectedDegree()
    {
        return this.selectedDegree;
    }

    public String getSelectedProgram() {
        return selectedProgram;
    }

    public void setSelectedProgram(String selectedProgram) 
            throws IOException {
        this.selectedProgram = selectedProgram;
        
    }
    
    
    /**
     *  Getter function for student's selection of courses in a list of strings.
     * @return List of the courses the student has chosen to execute.
     */
    public List<String> getSelectedCourses() 
    {
        return selectedCourses;
    }

    /**
     *  Lists the courses the student has marked as completed. 
     * @return The list of completed courses by the student.
     */
    public List<String> getCompletedCourses() 
    {
        return completedCourses;
    }

   /**
    * Adds a course as selected for student's list of future execution.
    * @param course - The course to be added to the list.
    */    
    public void addSelectedCourse(String course) 
    {
        this.selectedCourses.add(course);
    }
     
    /**
     * Removes a course from the student's list of future execution.
     * @param course The course to be deleted within the list.
     */
    public void removeSelectedCourse(String course) 
    {
        this.selectedCourses.remove(course);
    }
    
    /**
     * Adds a course as selected for student's list of completed courses.
     * @param course The course to be added to the list.
     */
    public void addCompletedCourse(String course) 
    {
        this.completedCourses.add(course);
    }
     
    /**
     * Removes a course from the student's list of completed courses.
     * @param course 
     */
    public void removeCompletedCourse(String course) 
    {
        this.completedCourses.remove(course);
    }
    

    /**
     * Getter function for student's completed credits.
     * @return The amount of credits already completed.
     */
    public int getCompletedCredits() 
    {
        return completedCredits;
    }
    
    /**
     * Adds student's completed credits.
     * @param amount The amount of credits to be added.
     */
    public void addCompletedCredits(int amount)
    {
        this.completedCredits += amount;
    }
    
    /**
     * Removes student's completed credits.
     * @param amount The amount of credits to be removed.
     */
    public void removeCompletedCredits(int amount)
    {
        this.completedCredits -= amount;
    }
    
    /**
     * Getter function for student's supposed graduating year.
     * @return The year the student has evaluated to be graduating in.
     */
    public int getExpectedGraduatingYear() 
    {
        return expectedGraduatingYear;
    }

    /**
     * Setter function for student's supposed graduating year.
     * @param expectedGraduatingYear The year the student evaluates to graduate.
     */
    public void setExpectedGraduatingYear(int expectedGraduatingYear) 
    {
        this.expectedGraduatingYear = expectedGraduatingYear;
    }
    
    public HashMap<String, CourseUnit> getCourseDatabase() {
        return courseDatabase;
    }

    public HashMap<String, Module> getModuleData() {
        return moduleData;
    }
    
    public boolean isProgramsChosen() {
        return programsChosen;
    }

    public void setProgramsChosen(boolean programsChosen) {
        this.programsChosen = programsChosen;
    }

    public List<String> getPrograms() {
        return programs;
    }

    public void setPrograms(List<String> programs) {
        this.programs = programs;
    }
    
    /**
     * Implements the readFromFile method to read data from JSON file.
     * @param fileName The file to be read.
     * @return Boolean - true if reading the file was successful.
     * @throws Exception if the file was not found.
     */
    @Override
    public boolean readFromFile(String fileName) throws Exception 
    {
        boolean readSuccess = false;
        try (BufferedReader reader = new BufferedReader(new FileReader(
                fileName))) 
        {
            Gson gson = new Gson();
            Type studentProgressType = new TypeToken<Student>(){}.getType();
            Student fromFile = gson.fromJson(reader, studentProgressType);
            if (fromFile != null)
            {
                this.name = fromFile.name;
                this.degreeGroupId = fromFile.degreeGroupId;
                this.studentNumber = fromFile.studentNumber;
                this.selectedDegree = fromFile.selectedDegree;
                this.selectedCourses = fromFile.selectedCourses;
                this.completedCourses = fromFile.completedCourses;
                this.expectedGraduatingYear = fromFile.expectedGraduatingYear;
                this.completedCredits = fromFile.completedCredits;
                readSuccess = true;
            }
        }
            catch (FileNotFoundException e) {
            // Handle the exception:
            System.err.println("File not found: " + fileName);
        }
        return readSuccess;
    }

    /**
     * Implements the writeToFile method to write data to JSON file.
     * @param fileName The name of the file to be written.
     * @return Boolean - true if the writing to the file was successful.
     * @throws Exception -exception
     */
    @Override
    public boolean writeToFile(String fileName) throws Exception 
    {
        boolean writeSuccess = false;
        // Create GSON builder
        GsonBuilder builder = new GsonBuilder();
        builder.setPrettyPrinting(); // Printout mod
        Gson gson = builder.create();

        // Convert to JSON
        String json = gson.toJson(this);

        // Save to JSON-file
        try 
        {
          FileWriter writer = new FileWriter(fileName);
          writer.write(json);
          writer.close();
          System.out.println("File saved successfully.");
          writeSuccess = true;
        } catch (IOException e) {
          System.out.println("Error while trying to save the file.");
          e.printStackTrace();
        }
        return writeSuccess;
    }
    
     @Override
     public JsonObject getJsonObjectFromApi(String urlString) {        
        // Create URL and connect to it
         JsonObject jsonObject = null;
        try {
            URL courseUrl = new URL(urlString);
            HttpURLConnection conn = (HttpURLConnection) 
                    courseUrl.openConnection();
            conn.setRequestMethod("GET");
            //conn.connect();

            // Check response code
            int responsecode = conn.getResponseCode();

            if (responsecode == 200) {
                //System.out.println("HttpResponseCode: " + responsecode);
                BufferedReader reader = new BufferedReader(
                        new InputStreamReader(conn.getInputStream()));
                StringBuilder response = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    response.append(line);
                }
                reader.close();
                
                JsonElement jsonElement = JsonParser.
                        parseString(response.toString());
                if (jsonElement.isJsonArray()) {
                    JsonArray jsonArray = jsonElement.getAsJsonArray();
                    jsonObject = jsonArray.get(0).getAsJsonObject();
                }else{
                    jsonObject = JsonParser.parseString(
                            response.toString()).getAsJsonObject();
                }
            }else{
                //System.out.println("Error: HttpResponseCode: " + responsecode);
            }    
            
            conn.disconnect();
            
        } catch (IOException e) {
            e.printStackTrace();
        }
        return jsonObject;
    }
    
     
     
     public void loadTree() throws IOException{
         this.getModuleRules(this.getSelectedDegree(), this.getSelectedDegree());
        JsonObject jsonObject = getJsonObjectFromApi(
                "https://sis-tuni.funidata.fi/kori/api/"
                + "modules/by-group-id?groupId=" + this.getSelectedDegree()
                + "&universityId=tuni-university-root-id");
        if (jsonObject == null) {
            return;
        }
        String jsonStr = jsonObject.toString();
        Gson gson = new Gson();
        
        JsonObject firstObject = gson.fromJson(jsonStr, JsonObject.class);
        if(jsonObject.isJsonArray()){
            JsonArray jsonArray = gson.fromJson(jsonStr, JsonArray.class);
            if(jsonArray != null){
                firstObject = gson.fromJson(jsonStr, JsonObject.class);
            }
        }
            JsonObject firstRule = firstObject.getAsJsonObject("rule");
            String text = "error";
            if (firstObject != null) {
                JsonObject name = firstObject.getAsJsonObject("name");
                if (name != null) {
                    if(name.has("fi")){
                        JsonPrimitive fi = name.getAsJsonPrimitive("fi");
                        text = fi.toString();
                    }
                }
            }
            if(!text.equals("")){
                rulesNextLevel(firstRule, text, "",true);
            }
     }
     
    /**
     * Lists all the possible degrees from the Sisu system by printing.
     * Due to be changed for GUI purposes later on.
     * @throws java.net.MalformedURLException
     */
    
    public HashMap<String, String> listDegreePrograms() 
            throws MalformedURLException, IOException {
        HashMap<String, String> hashMap = new HashMap<>();
    JsonObject jsonObject =
            
    getJsonObjectFromApi("https://sis-tuni.funidata.fi/kori/api/"
    + "module-search?curriculumPeriodId=uta-lvv-2021"
    + "&universityId=tuni-university-root-id"
    + "&moduleType=DegreeProgramme"
    + "&limit=1000");
   
    if (jsonObject == null) {
        return hashMap;
    }
        String jsonStr = jsonObject.toString();
    
        Gson gson = new Gson();

        JsonElement jelement = new JsonParser().parse(jsonStr);
        JsonObject jobject = jelement.getAsJsonObject();
        JsonArray jarray = jobject.getAsJsonArray("searchResults");

        for (var obj : jarray) {
            jobject = obj.getAsJsonObject();
            String name = jobject.get("name").getAsString();
            String groupId = jobject.get("groupId").getAsString();
            System.out.println(name + " / " + groupId);
            hashMap.put(name, groupId);
        }
        return hashMap;
    }
    
     
    
    private void addModuleRecursively(String name, String underWhat, 
            HashMap<String, HashMap<String, ArrayList<String>>> hashMap) {
        if (!underWhat.equals(name) && !name.equals("") 
                && !underWhat.equals("")) {
            if (!hashMap.containsKey(underWhat)) {
                hashMap.put(underWhat, new HashMap<>());
            }
            if (!hashMap.get(underWhat).containsKey(name)) {
                hashMap.get(underWhat).put(name, new ArrayList<>());
            }
        }
    }

    public void addCourseToModule(String moduleName, String courseName) {
        addTheValue(moduleName, courseName, this.modules);
    }
   
    public void addModuleToModule(String name, String underWhat, 
            HashMap<String, HashMap<String, ArrayList<String>>> modules) {
        if (!modules.containsKey(underWhat)) {
            modules.put(underWhat, new HashMap<>());
        }
        modules.get(underWhat).put(name, new ArrayList<>());
        
    }



    
    private void addTheValue(String keyToFind, 
            String valueToAdd, HashMap<String, ?> hashMap) {
        for (Map.Entry<String, ?> entry : hashMap.entrySet()) {
            if (entry.getKey().equals(keyToFind)) {
                if (entry.getValue() instanceof ArrayList) {
                    ArrayList<String> lista =
                            (ArrayList<String>) entry.getValue();
                    if (!lista.contains(valueToAdd)) { // Only unique courses
                        lista.add(valueToAdd);
                    }
                    return; // End recursive function
                } else if (entry.getValue() instanceof HashMap) {
                    // Dig deeper
                    addTheValue(keyToFind,
                            valueToAdd, (HashMap<String, ?>) entry.getValue());
                    return; // End recursive function
                }
            } else if (entry.getValue() instanceof HashMap) {
                // Dig deeper
                addTheValue(keyToFind, valueToAdd, 
                        (HashMap<String, ?>) entry.getValue());
            }
        }
    }
    
    public void printModuleHierarchy(HashMap<String, HashMap<String,
            ArrayList<String>>> modules, String moduleName, int level) {
        if (modules.containsKey(moduleName)) {
            HashMap<String, ArrayList<String>> module = modules.get(moduleName);
            for (String key : module.keySet()) {
                for (int i = 0; i < level; i++) {
                    System.out.print("--");
                }
                // Module
                System.out.println(key);

                ArrayList<String> courses = module.get(key);
                for (String course : courses) {
                    for (int i = 0; i < level + 1; i++) {
                        System.out.print("--");
                    }
                    // Course
                    System.out.println(">" + course + " groupId: " 
                            + getCourseDatabase().get(course).getGroupId());
                }
                // Recursive
                printModuleHierarchy(modules, key, level + 1);
            }
        }
    }

   
    public void listPrograms() throws IOException {
        
        
        System.out.println("Tason 1 moduulit:");
        for (String program : this.programs){
            
                System.out.println(program); 
        }
    }
    


    

    // Process the rules we found:
    public void processRule(JsonObject jsonObject, String name, 
            String underWhat)            
            throws IOException{
        if(jsonObject.isJsonArray()){
            jsonObject = jsonObject.getAsJsonArray().get(0).getAsJsonObject();
        }
        underWhat = underWhat.replace("\"", "");
        name = name.replace("\"", "");
        String type = jsonObject.has("type") ? 
               jsonObject.get("type").getAsString() : "";
        String localId = jsonObject.has("moduleGroupId") ? 
               jsonObject.get("moduleGroupId").getAsString() : "";
        String courseUnitGroupId = jsonObject.has("courseUnitGroupId") ? 
               jsonObject.get("courseUnitGroupId").getAsString() : "";     
        
        // Based on type
        switch (type) {
            case "CreditsRule":
               
                addModuleToModule(name, underWhat, this.modules);
                getModuleRules(localId,name);
                
                
                break;
            case "CompositeRule":
                String compareTo = this.getModuleName(
                    this.getSelectedDegree());
                if(!underWhat.equals(name) && !name.equals("") 
                        && this.getSelectedProgram().equals("")
                        && !name.equals(compareTo.replace("\"",""))
                        && !this.programs.contains(name)) {
                    this.programs.add(name);
                }
      
            if(!underWhat.equals(name) && !name.equals("") 
                    && !underWhat.equals("")
                    //&& !this.getSelectedProgram().equals("")
                    ){
                
                addModuleToModule(name, underWhat, this.modules);
                getModuleRules(localId,name);
            }
              
            break;
                      
            case "ModuleRule":
                
                addModuleToModule(name, underWhat, this.modules);
                getModuleRules(localId,name);
                break;  
            case "AnyModuleRule":
                addModuleToModule(name, underWhat, this.modules);
                getModuleRules(localId,name);
                break;
            case "CourseUnitRule":
                getCourseData(courseUnitGroupId,name);
                break;
            case "AnyCourseUnitRule":
               getCourseData(courseUnitGroupId,name);
                break;
                
            default:
                break;
        };
    }
    
    // Recursively lists the coursetree and sends found courses into processing
    public void rulesNextLevel(JsonElement jsonElement, String thisLevel, 
            String lastGroupId, boolean isRecursive) throws IOException {
         thisLevel = thisLevel.replace("\"", "");
        lastGroupId = lastGroupId.replace("\"", "");
        
        String moduleDescription = lastGroupId;
        if(jsonElement != null){
            if (jsonElement.isJsonObject()) {
                JsonObject jsonObject = jsonElement.getAsJsonObject();
                if (jsonObject.has("rules")) {
                    JsonArray rulesArray = jsonObject.getAsJsonArray("rules");
                    for (JsonElement ruleElement : rulesArray) {
                        if (ruleElement.isJsonObject()){
                            JsonObject ruleObject = 
                                    ruleElement.getAsJsonObject();
                            processRule(ruleObject, thisLevel, lastGroupId);
                            

                            if(ruleObject.has("rules") && isRecursive){
                                rulesNextLevel(jsonObject.get("rules"), 
                                        thisLevel, lastGroupId, true);

                            }
                            if(ruleObject.has("rule") && isRecursive){
                                rulesNextLevel(jsonObject.get("rule"), 
                                        thisLevel, lastGroupId, true);

                            }


                        }
                    }
                    if (jsonObject.has("rule")) {

                        JsonArray ruleArray = jsonObject.getAsJsonArray("rule");
                        for (JsonElement ruleElement : ruleArray) {
                            if (ruleElement.isJsonObject()) {
                                JsonObject ruleObject = 
                                        ruleElement.getAsJsonObject();
                                processRule(ruleObject, thisLevel,
                                        moduleDescription);
                                
                                if(ruleObject.has("rules") && isRecursive){
                                    rulesNextLevel(jsonObject.get("rules"),
                                            thisLevel, lastGroupId, true);

                                }
                                
                                if(ruleObject.has("rule") && isRecursive){
                                   // System.out.println("on");
                                    rulesNextLevel(jsonObject.get("rule"), 
                                            thisLevel, lastGroupId, true);
                                }
                            }
                        }
                    }
                }else{
                    // last level

                    if (jsonObject.has("rule")) {
                         JsonObject ruleObj = 
                                 jsonObject.getAsJsonObject("rule");
                        processRule(ruleObj,thisLevel,moduleDescription);
                        if(isRecursive){
                            rulesNextLevel(jsonObject.get("rule"), thisLevel, 
                                lastGroupId,true);                        
                        }
                    }
                }
            }else{
                JsonArray jsonArray = jsonElement.getAsJsonArray();
                for(JsonElement E : jsonArray){
                    if(E.getAsJsonObject().has("rules")){
                        JsonArray jsa = 
                                E.getAsJsonObject().get("rules").
                                        getAsJsonArray();
                        for(JsonElement E2 : jsa){
                            processRule(E2.getAsJsonObject(), thisLevel,
                                    moduleDescription);
                            if(isRecursive){
                                rulesNextLevel(E.getAsJsonObject().get("rules"), 
                                    thisLevel, lastGroupId, true);
                            }
                        }
                    }
                    if(E.getAsJsonObject().has("rule") && isRecursive){
                        rulesNextLevel(E.getAsJsonObject().get("rule"), 
                                thisLevel, lastGroupId, true);            
                    }
                }
            }
        }
     }
    
    
    
    
    // Reads the course data and 
    // adds it into the database under the correct module
    public void getCourseData(String courseUnitGroupId, String underWhat){
        JsonObject jsonObject = getJsonObjectFromApi(
                "https://sis-tuni.funidata.fi/kori/api/"
        + "course-units/by-group-id?groupId=" + courseUnitGroupId 
        + "&universityId=tuni-university-root-id");
        
        if (jsonObject == null) {
            return;
        }
        String jsonStr = jsonObject.toString();
        
        //gson
        Gson gson = new Gson();
        
        // Parse JSON-array
        JsonObject firstObject = gson.fromJson(jsonStr, JsonObject.class);
        if(jsonObject.isJsonArray()){
            JsonArray jsonArray = gson.fromJson(jsonStr, JsonArray.class);
            if(jsonArray != null){
                firstObject = gson.fromJson(jsonStr, JsonObject.class);
            }
        }
        
        // Name level:
        if (firstObject != null) {
            JsonObject name = firstObject.getAsJsonObject("name");
            if (name != null) {
                // fi level:
                if(name.has("fi")){
                    JsonPrimitive fi = name.getAsJsonPrimitive("fi");
                    String nameFi = fi.toString().replace("\"", "");
                    CourseUnit course = new CourseUnit(nameFi,"id",
                            courseUnitGroupId,0);
                    addTheValue(underWhat, nameFi, this.modules);
                    this.courseDatabase.put(nameFi, course);
                    
                }else{
                    JsonPrimitive en = name.getAsJsonPrimitive("en");
                    String nameEn = en.toString().replace("\"", "");
                    CourseUnit course = new CourseUnit(nameEn,"id",
                            courseUnitGroupId,0);
                    addTheValue(underWhat, nameEn, this.modules);            
                    this.courseDatabase.put(nameEn, course);
                }
            }
        }

    }
    
     // Get's the name of the Module from API
    public String getModuleName(String groupId){
        JsonObject jsonObject = getJsonObjectFromApi(
                "https://sis-tuni.funidata.fi/kori/api/"
                + "modules/by-group-id?groupId=" + groupId 
                + "&universityId=tuni-university-root-id");
        if (jsonObject == null) {
            return "error";
        }
        String jsonStr = jsonObject.toString();
        Gson gson = new Gson();
        String text = "error";
        JsonObject firstObject = gson.fromJson(jsonStr, JsonObject.class);
        if(jsonObject.isJsonArray()){
            JsonArray jsonArray = gson.fromJson(jsonStr, JsonArray.class);
            if(jsonArray != null){
                firstObject = gson.fromJson(jsonStr, JsonObject.class);
            }
        }
            JsonObject firstRule = firstObject.getAsJsonObject("rule");
            JsonObject name = firstObject.getAsJsonObject("name");
            if (name != null) {
                if(name.has("fi")){
                    JsonPrimitive fi = name.getAsJsonPrimitive("fi");
                    text = fi.toString();
                     return text;
                }else{
                    JsonPrimitive en = name.getAsJsonPrimitive("en");
                    text = en.toString();
                    return text;
                }
            }
        
        return text;
    }
    
    // Recursively goes though the modules to gather all the assosiated rules.
    public void getModuleRules(String groupId, String lastGroupId) 
            throws MalformedURLException, IOException {
        JsonObject jsonObject = getJsonObjectFromApi(
                "https://sis-tuni.funidata.fi/kori/api/"
                + "modules/by-group-id?groupId=" + groupId 
                + "&universityId=tuni-university-root-id");
        if (jsonObject == null) {
            return;
        }
        String jsonStr = jsonObject.toString();
        Gson gson = new Gson();
        if(lastGroupId.equals("")){
            lastGroupId = getModuleName(groupId);
        }
        
        JsonObject firstObject = gson.fromJson(jsonStr, JsonObject.class);
        if(jsonObject.isJsonArray()){
            JsonArray jsonArray = gson.fromJson(jsonStr, JsonArray.class);
            if(jsonArray != null){
                firstObject = gson.fromJson(jsonStr, JsonObject.class);
            }
        }
            JsonObject firstRule = firstObject.getAsJsonObject("rule");
            String text = "error";
            if (firstObject != null) {
                JsonObject name = firstObject.getAsJsonObject("name");
                if (name != null) {
                    if(name.has("fi")){
                        JsonPrimitive fi = name.getAsJsonPrimitive("fi");
                        text = fi.toString();
                        
                        //            JsonObject jsonArrayData = jsonObject.getAsJsonArray().get(0).getAsJsonObject();
        //Module module = gson.fromJson(jsonObject, Module.class);
        //this.moduleData.put(text,module);
                    }
                }
                
            }

            // this is the first level after degree:
            processRule(firstRule, text, lastGroupId);
            // Go through the tree recursively to find all the assosiated rules:
            if(this.programsChosen){
                rulesNextLevel(firstRule, text, lastGroupId, true);
            }else{
                rulesNextLevel(firstRule, text, lastGroupId, false);
            }
    }
    
    public HashMap<String, HashMap<String, ArrayList<String>>> getModules() {
        return this.modules;
    }

    
    /*
    To test the class:
    public static void main(String[] args) throws IOException {
        Student testi = new Student("name", 12345);

        // Gets the list the degrees
  //      HashMap<String, String> degreeList = testi.listDegreePrograms();
        // This sets one degree.
        //testi.setSelectedDegree("tut-dp-g-1280");
        // This gets the list of programs
    //    testi.listPrograms();
        
        // This sets one program
        //testi.setSelectedProgram("Tuotantotalous");
        // This gets the wholetree


        //testi.printModuleHierarchy(testi.getModules(), testi.getSelectedDegree(), 0); 

        // This gets the filtered tree
    //    testi.printModuleHierarchy(testi.getModules(), testi.getSelectedProgram(), 0); 
    }
    */
    
   

    
}
