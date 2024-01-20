package fi.tuni.prog3.sisu;
import java.util.ArrayList;
import java.util.List;


/**
 *Abstract class
 * @author kalle
 */
public abstract class Module {
    private String name;
    private String id;
    private String groupId;
    private Integer minCredits;
    private List<Module> modules;
    private Boolean isSelected;
    

    public Module(String name, String id, String groupId, 
            int minCredits) {
        
        this.name = name;
        this.id = id;
        this.groupId = groupId;
        this.minCredits = minCredits;
        this.modules = new ArrayList<>();
        this.isSelected = false;
    }
    

    /**
     * Getter function for module's name
     * @return Module's name
     */
    public String getName() {
        return this.name;
    }
    
    /**
     * Getter function for module's id
     * @return Module's id
     */
    public String getId() {
        return this.id;
    }
    

    /**
     * Getter function for module's group id
     * @return Module's group Id
     */
    public String getGroupId() {
        return this.groupId;
    }
    

    /**
     * Getter function for module's credits
     * @return The amount of credits 
     */
    public int getMinCredits() {
        return this.minCredits;
    }

    /**
     * Getter function for module's 
     * @return list of modules under this module
     */
    public List<Module> getModules() {
        return modules;
    }

    /**
     * Getter function for isSelected
     * @return is the module selected
     */
    public Boolean getIsSelected() {
        return isSelected;
    }

    /**
     * Setter function for isSelected
     * @param isSelected Boolean value that determines is the module selected
     */
    public void setIsSelected(Boolean isSelected) {
        this.isSelected = isSelected;
    }

   
    
}
