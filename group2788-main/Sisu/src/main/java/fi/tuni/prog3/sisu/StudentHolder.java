package fi.tuni.prog3.sisu;

/**
 * Class to pass information between studentController and CourseController
 * using instance
 * @author Joonas Kärnä
 */
public class StudentHolder {
    
    private final static StudentHolder instance = new StudentHolder();
    
    private Student student;
    private String studentNumber;
    
    private StudentHolder() {};
    
    /**
     * Function for saving an instance of studentHolder object
     * @return instance of StudentHolder object
     */
    public static StudentHolder getInstance()
    {
        return instance;
    }
    /**
     * Setting student that information is passed
     * @param s which information is passed
     */
    public void setStudent(Student s)
    {
        this.student = s;
    }
    
    /**
     * Return student from instance
     * @return student Object
     */
    public Student getStudent()
    {
        return this.student;
    }
    /**
     * Option to set StudentNumber for instance of a student
     * @param s -string that is passed as student number
     */
    public void setStudentNumber(String s)
    {
        this.studentNumber = s;
    }
    /**
     * Returns the instances students number
     * @return String studentNumber of instance
     */
    public String getStudentNumber()
    {
        return this.studentNumber;
    }
}
