package fi.tuni.prog3.sisu;
/**
 *Class for a single course, extends Module
 * @author kalle
 */
public class CourseUnit extends Module{

    private String courseName;
    private String courseId;
    private String courseGroupId;
    private int courseMinCredits;
    
    public CourseUnit(String name, String id, String groupId, int minCredits) {
        super(name, id, groupId, minCredits);
        this.courseName = name;
        this.courseId = id;
        this.courseGroupId = groupId;
        this.courseMinCredits = minCredits;   
    }
    
    
    @Override
    public String getName() {
        return this.courseName;
    }

    public void setName(String name) {
        this.courseName = name;
    }

    @Override
    public String getId() {
        return this.courseId;
    }

    public void setId(String id) {
        this.courseId = id;
    }

    @Override
    public String getGroupId() {
        return this.courseGroupId;
    }

    public void setGroupId(String groupId) {
        this.courseGroupId = groupId;
    }

    @Override
    public int getMinCredits() {
        return courseMinCredits;
    }

    public void setMinCredits(int minCredits) {
        this.courseMinCredits = minCredits;
    }

}
