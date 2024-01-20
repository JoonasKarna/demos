package fi.tuni.prog3.sisu;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 *
 * @author ASRock
 */
public class StudentTest {
    
    public StudentTest() {
    }
    
    @BeforeAll
    public static void setUpClass() {
    }
    
    @AfterAll
    public static void tearDownClass() {
    }
    
    @BeforeEach
    public void setUp() {
    }
    
    @AfterEach
    public void tearDown() {
    }

    /**
     * Test of getName method, of class Student.
     */
    @Test
    public void testGetName() {
        System.out.println("getName");
        Student instance = new Student("Teemu Teekkari",1234);
        String expResult = "Teemu Teekkari";
        String result = instance.getName();
        assertEquals(expResult, result);
    }

    /**
     * Test of setName method, of class Student.
     */
    @Test
    public void testSetName() {
        System.out.println("setName");
        String name = "Teemu Teekkari";
        Student instance = new Student();
        instance.setName(name);
        String expResult = "Teemu Teekkari";
        String result = instance.getName();
        assertEquals(expResult, result);
    }

    /**
     * Test of getStudentNumber method, of class Student.
     */
    @Test
    public void testGetStudentNumber() {
        System.out.println("getStudentNumber");
        Student instance = new Student("Teemu Teekkari",1234);
        int expResult = 1234;
        int result = instance.getStudentNumber();
        assertEquals(expResult, result);
    }

    /**
     * Test of setStudentNumber method, of class Student.
     */
    @Test
    public void testSetStudentNumber() {
        System.out.println("setStudentNumber");
        int studentNumber = 1234;
        Student instance = new Student();
        instance.setStudentNumber(studentNumber);
        int result = instance.getStudentNumber();
        int expResult = 1234;
        assertEquals(expResult, result);
    }

    /**
     * Test of setSelectedDegree method, of class Student.
     */
    @Test
    public void testSetSelectedDegree() throws IOException {
        System.out.println("setSelectedDegree");
        String degree = "Tietojenkäsittelytiede";
        Student instance = new Student();
        instance.setSelectedDegree(degree);
        String expResult = "Tietojenkäsittelytiede";
        String result = instance.getSelectedDegree();
        assertEquals(expResult, result);
    }

    /**
     * Test of getSelectedCourses method, of class Student.
     */
    @Test
    public void testGetSelectedCourses() {
        System.out.println("getSelectedCourses");
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addSelectedCourse("kurssi1");
        instance.addSelectedCourse("kurssi2");
        instance.addSelectedCourse("kurssi3");
        List<String> expResult = Arrays.asList(
                "kurssi1","kurssi2","kurssi3");
        List<String> result = instance.getSelectedCourses();
        assertEquals(expResult, result);
    }

    /**
     * Test of getCompletedCourses method, of class Student.
     */
    @Test
    public void testGetCompletedCourses() {
        System.out.println("getCompletedCourses");
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addCompletedCourse("kurssi1");
        instance.addCompletedCourse("kurssi2");
        instance.addCompletedCourse("kurssi3");
        
        List<String> expResult = Arrays.asList(
                "kurssi1","kurssi2","kurssi3");
        List<String> result = instance.getCompletedCourses();
        assertEquals(expResult, result);
    }

    /**
     * Test of addSelectedCourse method, of class Student.
     */
    @Test
    public void testAddSelectedCourse() {
        System.out.println("addSelectedCourse");
        String course = "kurssi1";
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addSelectedCourse(course);
        List<String> result = instance.getSelectedCourses();
        List<String> expResult = Arrays.asList("kurssi1");
        assertEquals(expResult,result);
    }

    /**
     * Test of removeSelectedCourse method, of class Student.
     */
    @Test
    public void testRemoveSelectedCourse() {
        System.out.println("removeSelectedCourse");
        List<String> expResult = Arrays.asList(
                "kurssi1","kurssi3");
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addSelectedCourse("kurssi1");
        instance.addSelectedCourse("kurssi2");
        instance.addSelectedCourse("kurssi3");
        instance.removeSelectedCourse("kurssi2");
        assertEquals(expResult,instance.getSelectedCourses());
    }

    /**
     * Test of addCompletedCourse method, of class Student.
     */
    @Test
    public void testAddCompletedCourse() {
        System.out.println("addCompletedCourse");
        String course = "Kurssi1";
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addCompletedCourse(course);
        List<String> expResult = Arrays.asList("Kurssi1");
        assertEquals(expResult, instance.getCompletedCourses());      
    }

    /**
     * Test of removeCompletedCourse method, of class Student.
     */
    @Test
    public void testRemoveCompletedCourse() {
        System.out.println("removeCompletedCourse");
        Student instance = new Student("Teemu Teekkari",1234);
        
        instance.addCompletedCourse("Kurssi1");
        instance.addCompletedCourse("Kurssi2");
        instance.addCompletedCourse("Kurssi3");
        
        List<String> expResult = Arrays.asList("Kurssi1","Kurssi3");
        instance.removeCompletedCourse("Kurssi2");
        assertEquals(expResult, instance.getCompletedCourses());
    }

    /**
     * Test of getCompletedCredits method, of class Student.
     */
    @Test
    public void testGetCompletedCredits() {
        System.out.println("getCompletedCredits");
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addCompletedCredits(5);
        instance.addCompletedCredits(5);
        int expResult = 10;
        int result = instance.getCompletedCredits();
        assertEquals(expResult, result);
    }

    /**
     * Test of addCompletedCredits method, of class Student.
     */
    @Test
    public void testAddCompletedCredits() {
        System.out.println("addCompletedCredits");
        int amount = 5;
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addCompletedCredits(amount);
        int expResult = 5;
        int result = instance.getCompletedCredits();
        assertEquals(expResult,result);
    }

    /**
     * Test of removeCompletedCredits method, of class Student.
     */
    @Test
    public void testRemoveCompletedCredits() {
        System.out.println("removeCompletedCredits");
        int amount = 5;
        Student instance = new Student("Teemu Teekkari",1234);
        instance.addCompletedCredits(10);
        instance.removeCompletedCredits(amount);
        int expResult = 5;
        int result = 5;
        assertEquals(expResult,result);
        
    }

    /**
     * Test of getExpectedGraduatingYear method, of class Student.
     */
    @Test
    public void testGetExpectedGraduatingYear() {
        System.out.println("getExpectedGraduatingYear");
        Student instance = new Student("Teemu Teekkari",1234);
        instance.setExpectedGraduatingYear(2025);
        int expResult = 2025;
        int result = instance.getExpectedGraduatingYear();
        assertEquals(expResult, result);
    }

    /**
     * Test of setExpectedGraduatingYear method, of class Student.
     */
    @Test
    public void testSetExpectedGraduatingYear() {
        System.out.println("setExpectedGraduatingYear");
        int expectedGraduatingYear = 2025;
        Student instance = new Student("Teemu Teekkari",1234);
        instance.setExpectedGraduatingYear(expectedGraduatingYear);
        int expResult = 2025;
        int result = instance.getExpectedGraduatingYear();
        assertEquals(expResult,result);
    }


}
