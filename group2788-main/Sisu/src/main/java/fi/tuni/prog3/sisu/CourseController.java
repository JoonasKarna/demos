package fi.tuni.prog3.sisu;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import javafx.beans.property.ReadOnlyStringWrapper;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.Tooltip;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeTableColumn;
import javafx.scene.control.TreeTableRow;
import javafx.scene.control.TreeTableView;
import javafx.scene.input.MouseEvent;

/**
 *  Course controller that is a main window to show the user chosen student
 *  name, degree, programme, student number and other info. Also makes it
 *  possible for the user to check degree in a treeView
 *  @author Joonas Kärnä
 */
public class CourseController {
    //student holder object to pass infromation from one controller to another
    StudentHolder studentData = StudentHolder.getInstance();
    
    //using the information of the student from the file
    Student studentObj = studentData.getStudent();
    
    //saving the course credits
    private int courseCredits = 0;
    //saving the chosen programme
    private String chosenProgramme = "";
    //saving the degree name
    private String degreeName = "";
    //saving the degree groupId
    private String degreeGroupId = "";
    //saving the programme  name
    private String programmeName = "";
    //MenuButton to show different degrees
    @FXML
    private MenuButton degreesMenuButton;
    //Button for showing programs according to the degree
    @FXML
    private Button programsButton;
    //show the infromation of the course status on courses tab
    @FXML
    private Label courseStatusLabel; 
    //show the course name on the progress tab
    @FXML
    private Label courseNameLabel;
    //Label that shows the current studentProgramme
    @FXML
    private Label studentProgrammeLabel;
    //label that shows the students number
    @FXML
    private Label studentNumberLabel;
    //label that shows the students name
    @FXML
    private Label studentNameLabel;
    //label that shows the students degree
    @FXML
    private Label studentDegreeLabel;
    //TableView that shows the students completed courses
    @FXML
    private TableView<String> progressTableView;
    //column that shows the individual course
    @FXML
    private TableColumn<String,String>progressColumn;
    //shows the status for the course "completed" if course is completed
    @FXML
    private TableColumn<String,String> statusColumn;
    //shows the programmes that the degree has
    @FXML
    private TableView<String> programmesTable;
    //shows the programmes that are available for the degree
    @FXML
    private TableColumn<String,String> programmesColumn;
    //Shows tree like structure for the selected degree
    @FXML
    private TreeTableView<String> coursesTreeTable;
    //column that the courses and modules are used
    @FXML
    private TreeTableColumn<String, String> coursesColumn;
    //shows the amount of credits completed
    @FXML
    private Label completedStudiesLabel;
    //input field for student name
    @FXML
    private TextField studentNameInput;
    //input field for student graduating year
    @FXML
    private TextField studentGYearInput;
    //input field for student number
    @FXML
    private TextField studentNumberInput;
    //shows the students graduating year
    @FXML
    private Label expectedGYearLabel;
    /**
     * initializing when user opens Course Controller
     * @throws MalformedURLException
     * @throws IOException -ioexception
     */
    @FXML
    private void initialize() throws MalformedURLException, IOException
    {
        //initialising individual student number and name labels
        studentNumberLabel.setText(String.valueOf(
                studentObj.getStudentNumber()));
        studentNameLabel.setText(studentObj.getName());
        expectedGYearLabel.setText(Integer.toString(
                studentObj.getExpectedGraduatingYear()));
        this.degreeGroupId = studentObj.getDegreeGroupId();
        if(studentObj.getSelectedDegree().isEmpty())
        {
            studentDegreeLabel.setText("No chosen programme");
        }
        else
        {
            studentDegreeLabel.setText(studentObj.getSelectedDegree());
        }
        //we also fill the programs button menu
        fillProgramsButton();
        
    }
    /**
     * The function saves the students settings
     */
    @FXML private void saveStudentSettings()
    {
        String studentNameStr = studentNameInput.getText();
        String GraduatingYearStr = studentGYearInput.getText();
        String studentNumberStr = studentNumberInput.getText();
        
        if(!studentNameStr.equals(""))
        {
            studentObj.setName(studentNameStr);
        }
        if(!GraduatingYearStr.equals(""))
        {
            try
            {
                int gYear = Integer.parseInt(GraduatingYearStr);
                studentObj.setExpectedGraduatingYear(gYear);
            }
            catch (Exception e)
            {
                System.out.println("Graduating year not a number");
            }
        }
        if(!studentNumberStr.equals(""))
        {
            try
            {
                int studentNumb = Integer.parseInt(studentNumberStr);
                studentObj.setStudentNumber(studentNumb);
            }
            catch (Exception e)
            {
                System.out.println("StudentNumber not a number");
            }
        }
        studentNameInput.clear();
        studentGYearInput.clear();
        studentNumberInput.clear();
                
    }
    /**
     * Clears the table and chosen programme when the user selects another
     * degree also makes it impossible for the user choose another degree and
     * submodules from previous degree
     * 
     */
    @FXML
    private void clearPrograms()
    {
        studentProgrammeLabel.setText("No chosen programme");
        programmesTable.getItems().clear();
    }
    /**
     * The function takes individual data from the table cell that the user
     * chooses
     * @param event -MouseEvent that triggers when column item is clicked 
     */
    @FXML
    private void handleTableViewClick(MouseEvent event)
    {
        //check that only one click is registered
        if (event.getClickCount() == 1)
        {
            String cellData = programmesColumn.getCellObservableValue(
                    programmesTable.getSelectionModel()
                            .getSelectedIndex()).getValue();
            //give label a value programs name by using the programmeName holder
            this.programmeName = cellData;
            studentProgrammeLabel.setText(this.programmeName);
        }
    }
    /**
     * We Check that if the current chosen degree has programs under it and
     * then we show those to the user
     * @throws IOException -ioexception
     */
    @FXML
    private void showPrograms() throws IOException
    {   
        
        if(studentDegreeLabel.getText().equals("No chosen programme"))
        {
            System.out.println("Ei valittu degreetä");
            return;
        }
        System.out.println("oppilaan group id "+this.degreeGroupId);
        System.out.println("oppilaan degree name "+this.degreeName);
        //we create temporary a new student object where current printing
        //functions are located
        Student temporaryObj = new Student("temporary",0);
        //we select the current degree for the temporary object
        temporaryObj.setSelectedDegree(this.degreeGroupId);
        System.out.println("set selected degree jälkeen");

        ArrayList<String> programs = (ArrayList<String>) temporaryObj.getPrograms();
        //check if there are at least one sub program
        if(programs.size() > 0)
        {
            fillProgammesTable(programs);
        }
        else
        {
            programmesTable.setItems(FXCollections.emptyObservableList());
            System.out.println("No sub programs");
        }
        
    }
    /**
     * Function for add course button for adding the course from the courses tab
     * course is added to the students selected courses array if it is not
     * already there.
     */
    @FXML
    private void addCourse()
    {
        //check that we are not adding to courses no real course
        if(courseNameLabel.getText().equals("No chosen course"))
        {
            return;
        }
        //check if course already exists in students selected courses
        if(studentObj.getSelectedCourses().contains(courseNameLabel.getText()))
        {
            courseStatusLabel.setText("Course already in selected courses");
            return;
        }
        //add course
        studentObj.addSelectedCourse(courseNameLabel.getText());
        courseStatusLabel.setText("Course added");
    }
    /**
     * function that completes the course for the student and adds the credits
     * to the students credit saldo
     */
    @FXML
    private void completeCourse()
    {
        String courseName = courseNameLabel.getText();
        //check that there is a chosen course
        if(courseNameLabel.getText().equals("No chosen course"))
        {
            return;
        }
        //check that the course is not already completed
        if(!studentObj.getCompletedCourses().contains(courseName))
        {
            //add the course to completed courses
            studentObj.addCompletedCourse(courseName);
            //add the credits to the student object
            studentObj.addCompletedCredits(this.courseCredits);
        }
    }
    /**
     * removes the chosen course from the students selected courses list
     * if it is not completed
     * @throws IOException -ioexception
     */
    @FXML
    private void removeCourse() throws IOException
    {
        // get the selected course from the progressTableView
        int selectedIndex = progressTableView.getSelectionModel()
                .getSelectedIndex();
        String course = progressColumn.getCellObservableValue(selectedIndex)
                .getValue();
        // remove the course from the selectedCourses list
        if(!studentObj.getCompletedCourses().contains(course))
        {
            studentObj.removeSelectedCourse(course);
            // remove the corresponding row from the progressTableView
            progressTableView.getItems().remove(selectedIndex);
        }
    }

    
    /**
     * Function for the showProgress Button shows the students courses and
     * status "complete" if the student has completed the course
     * also has the hover ability to show more information of the course
     * @throws IOException -ioexception
     */
    @FXML
    private void showProgress() throws IOException
    {
        ObservableList<String> selectedCourses = 
                FXCollections.observableArrayList();
        progressTableView.setItems(selectedCourses);
        //find all the selected courses
        for(String course : studentObj.getSelectedCourses())
        {
            selectedCourses.add(course);
        }
        //get access to individual row
        progressColumn.setCellValueFactory(cellData -> 
                new SimpleStringProperty(cellData.getValue()));
        statusColumn.setCellValueFactory(cellData -> {
            String element = cellData.getValue();
            //if course is completed then add "completed" to next column
            if(studentObj.getCompletedCourses().contains(element))
            {
                return new SimpleStringProperty("Completed");
            }
            else
            {
                return new SimpleStringProperty("");
            }
        });    
        completedStudiesLabel.setText("Completed credits so far: "+studentObj.getCompletedCredits());
        
        progressTableView.setItems(selectedCourses);
    }   
    /**
     * finds the course by name from the database and returns a course object
     * which is used to show more information from the course
     * @param courseName -courses name that we find the course group id
     * @param studentObj -student obj that has the database of the courses
     * @return course -CourseUnit object that contains the selected course info
     * @throws MalformedURLException
     * @throws IOException -ioexception
     */
    private CourseUnit findCourseByGroupId(String courseName, Student studentObj) 
            throws MalformedURLException, IOException
    {
        //find the course group id from the course database that is in the class
        String courseGroupId = studentObj.getCourseDatabase()
                .get(courseName).getGroupId();
        
        //attributes for the courseUnit object
        String courseId = "";
        int minCredits = 0;
        String desc = "";
        
        try
        {
            //URL where the course info is located
            var url = new URL(
                    "https://sis-tuni.funidata.fi/kori/api/course-units/by-group-id?groupId="+
                courseGroupId+"&universityId=tuni-university-root-id");
            //data in raw json form
            String data = new String(url.openStream().readAllBytes()); 
            Gson gson = new Gson();
            //course data is in json array so we parse it through to get
            //individual values
            JsonArray jsonArray = JsonParser.parseString(data).getAsJsonArray();
            JsonObject jsonObject = jsonArray.get(0).getAsJsonObject();
        
            JsonElement id = jsonObject.get("id");
            JsonObject creditObj = (JsonObject) jsonObject.get("credits");
            JsonElement min = creditObj.get("min");

            courseId = id.getAsString();
            minCredits = min.getAsInt();   
        }
        catch (IOException e)
        {
            System.out.println(e);
        }
        //creation a course object and passing it to the caller
        CourseUnit course = new CourseUnit(courseName,courseId,
                courseGroupId, minCredits);
        return course;
    } 
    /**
     * returns to the login window and saves the students info into file
     * @throws IOException -ioexception
     * @throws Exception 
     */
    @FXML
    private void logout() throws IOException, Exception
    {
        try{
            if(!studentDegreeLabel.getText().equals("No chosen degree"))
            {
                studentObj.setSelectedDegree(studentDegreeLabel.getText());
            }
            if(!studentProgrammeLabel.getText().equals("No chosen programme"))
            {
                studentObj.setSelectedProgram(studentProgrammeLabel.getText());
            }
            //talletetaan uusi degreegroupid
            studentObj.setDegreeGroupId(this.degreeGroupId);
            System.out.println("Student degree "+studentObj.getSelectedDegree());
            studentObj.writeToFile("test.json");
            Sisu.setRoot("student");
        }
        catch (Exception e)
        {
            System.out.println("Couldnt save to a file");
            System.out.println(e);
        }
        
    }
    
    /**
     * presents the students chosen degreeStructure in treetableViw
     * it either shows the whole structure or structure of the programme/module
     * that the user has chosen. Takes some time to load depending on degree
     * @throws IOException -ioexception
     */
    @FXML
    private void ShowDegreeStructure() throws IOException
    {  
        //get the degree names so we know what to search
        
        String degreeNameStr = studentDegreeLabel.getText();
        String programmeNameStr = studentProgrammeLabel.getText();
        System.out.println(this.degreeGroupId);
        
        //first root that we put the whole structure
        TreeItem<String> rootItem = new TreeItem<>("Structure of courses");
        coursesTreeTable.setRoot(rootItem);
        coursesColumn.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<String, String> p) ->
                        new ReadOnlyStringWrapper(p.getValue().getValue()));

        
        if(degreeNameStr.equals("No chosen programme"))
        {
            return;
        }
        
        //use temporary student object to 
        
        Student temp = new Student("temp",0);
        //if no degree is chosen then we simply do nothing
        temp.setSelectedDegree(this.degreeGroupId);
        if(!degreeNameStr.equals("No chosen programme") &&
                !programmeNameStr.equals("No chosen programme"))
        {
            //set the programme for temp object and print it
            temp.setSelectedProgram(programmeNameStr);
            printModuleHierarchy(temp.getModules(),
                    temp.getSelectedProgram(),0,rootItem);
            
        }
        else if(!degreeNameStr.equals("No chosen programme"))
        {
            System.out.println("Degree name löytyy toista ei");
            printModuleHierarchy(temp.getModules(),temp.getSelectedDegree(),0,rootItem); 
        }
         
        //creation of functionality for the leaf
        coursesTreeTable.getSelectionModel().selectedItemProperty()
                .addListener((observable, oldSelection, newSelection) -> 
        {
            if (newSelection != null && newSelection.isLeaf())
            {
                // A course item was selected
                String course = newSelection.getValue();
                courseNameLabel.setText(course);
                System.out.println("Selected course: " + course);
                try
                {
                    CourseUnit courseObj = findCourseByGroupId(course, temp);
                    System.out.println(courseObj.getMinCredits());
                    this.courseCredits = courseObj.getMinCredits();
                }
                catch(IOException ex)
                {
                    System.out.println(ex);
                }
                
            }
        });
        //creation of hover tooltip
        coursesTreeTable.setRowFactory(tv ->
        {
            TreeTableRow<String> row = new TreeTableRow<>();
            Tooltip tooltip = new Tooltip();
            row.setOnMouseEntered(event ->
            {
                if (! row.isEmpty() && row.getTreeItem().isLeaf())
                {
                    try
                    {
                        //find information of the course for the tooltip 
                        String course = row.getItem();
                        CourseUnit courseObj = findCourseByGroupId(course, temp);
                        
                        tooltip.setText("Course: " + course+"\n"
                        +"GroupId: "+courseObj.getGroupId()+"\n"
                        +"Min credits: "+courseObj.getMinCredits()+"\n");
                        
                        tooltip.show(row, event.getSceneX(), event.getSceneY());

                    }
                    catch (IOException ex)
                    {
                        System.out.println(ex);
                    }
                }
            });
            row.setOnMouseExited(event ->
            {
                tooltip.hide();
            });
            return row;
        });

        
    }
    /**
     * switches to the progress tab
     * @throws IOException -ioexception
     */
    @FXML
    private void swtichToProgress() throws IOException
    {
        System.out.println("switch back to progress tab");
    }
    /**
     * Function to show degrees on the MenuButton used in initialization of the
     * course controller
     * @throws IOException -exception
     */
    private void fillProgramsButton() throws IOException
    {
        //using a temporary Student object to not to mess with the current
        Student temporaryObj = new Student();
        HashMap<String, String> degreeList =
                temporaryObj.listDegreePrograms();
        for(Map.Entry<String, String> degreeEntry : degreeList.entrySet())
        {
            //adding all the degrees as menu items
            MenuItem menuItem = new MenuItem(degreeEntry.getKey());
            menuItem.setOnAction(e -> {
                studentDegreeLabel.setText(degreeEntry.getKey());
                this.degreeName = degreeEntry.getKey();
                this.degreeGroupId = degreeEntry.getValue();
            });
            degreesMenuButton.getItems().add(menuItem);
        }
    }
    /**
     * Function to fill programmesTable with programmes if these exist for
     * the degree
     * @param programmesArray -Array that contains all the programmes
     */
    private void fillProgammesTable(ArrayList<String> programmesArray)
    {
        ObservableList<String> obsList = FXCollections.observableArrayList();
        for(String programme : programmesArray)
        {
            if(programme != "error")
            {
                obsList.add(programme);
            }          
        }
        
        programmesColumn.setCellValueFactory(
                cellData -> new SimpleStringProperty(cellData.getValue()));
        //adding all the programs to the table        
        programmesTable.setItems(obsList);
    }  
    /**
     * Fills the courses tab treetableview if the current sub program is known
     * @param modules -the modules that the degree has
     * @param moduleName -user chosen idnividual module
     * @param level -current level that the recursion is
     * @param parentItem -treenode that the whole treestructure is added
     */
    public void printModuleHierarchy(
            HashMap<String, HashMap<String, ArrayList<String>>> modules,
            String moduleName, int level, TreeItem<String> parentItem)
    {
        if (modules.containsKey(moduleName))
        {
            HashMap<String, ArrayList<String>> module = modules.get(moduleName);
            for (String key : module.keySet())
            {
            // Create a TreeItem for the module
                TreeItem<String> moduleItem = new TreeItem<>(key);
                parentItem.getChildren().add(moduleItem);

                ArrayList<String> courses = module.get(key);
                for (String course : courses)
                {
                    // Create a TreeItem for the course
                    TreeItem<String> courseItem = new TreeItem<>(course);
                    moduleItem.getChildren().add(courseItem);           
                }
            // Recursive
            printModuleHierarchy(modules, key, level + 1, moduleItem);
            }
        }
    }


    
}
