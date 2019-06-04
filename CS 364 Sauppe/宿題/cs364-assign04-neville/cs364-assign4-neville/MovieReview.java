import java.sql.*;
import java.util.*;
import java.io.*;
import java.text.*;
public class MovieReview {
    
    private static Scanner sn;
    private static Connection connection;  

    public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException {
        // Load the JDBC driver
        Class.forName("org.mariadb.jdbc.Driver");
        connection = connect();
        sn = new Scanner(System.in);
        String user = "movieJunkie";
        int opt = 0;
        while(opt != 6){
            System.out.print("Would you like to...\n" +
                                "\t\t 1. Add a movie?\n" +
                                "\t\t 2. Add an actor?\n" +
                                "\t\t 3. List the top earning movies?\n" +
                                "\t\t 4. Get information and reviews for a movie?\n" +
                                "\t\t 5. Write a new review? (currently logged in as" + user + "\n" +
                                "\t\t 6. Logout of this session?\n\n" +
                                "Enter your option here: ");
            opt = sn.nextInt();
            switch(opt){
                case 1: addMovie();
                        break;
                case 2: addActor();
                        break;
                case 3: listTop();
                        break;
                case 4: getInfo();
                        break;
                case 5: writeReview();
                        break;
                default: //logout of session
                        break;
            }
        }
        System.out.print("\nThank you for using the movie review system!");
        connection.close();
    }
    
    public static void writeReview() throws SQLException{
        System.out.println("Which movie would you like to write a review for? ");
        String movie = sn.nextLine();
        String queryStmt = "SELECT * " + 
                           "FROM Movie" +
                           "WHERE Title LIKE ?;";
        PreparedStatement query = connection.prepareStatement(queryStmt);
        query.setString(1, movie);
        ResultSet rs = query.executeQuery();
        int id = rs.getInt("MID");
        System.out.println("Please provide your review here: ");
        String review = sn.nextLine();
        String insertStmt = "INSERT INTO Reviews (Username, MID, Review, Posted) VALUES (? ? ? ?);";
        PreparedStatement insert = connection.prepareStatement(insertStmt);
        insert.setString(1, "movieJunkie");
        insert.setInt(2, id);
        insert.setString(3, review);
        SimpleDateFormat f = new SimpleDateFormat("yyy-MM-dd");
        String date = f.format(Calendar.getInstance().getTime());
        insert.setString(4, date); 
    }   

    public static void getInfo() throws SQLException{
        System.out.println("Which movie would you like information on? ");
        String movie = sn.nextLine();   
        String queryStmt =  "SELECT * " +
                            "FROM Movie " +
                            "WHERE Title LIKE ? ;";
        PreparedStatement query = connection.prepareStatement(queryStmt);
        ResultSet rs = query.executeQuery();
        String title = rs.getString("Title");
        String date = rs.getString("DateReleased");
        DecimalFormat formatter = new DecimalFormat("#,###.00");
        String gross = formatter.format(rs.getDouble("Gross"));
        System.out.println(title + "(released: " + date + "; grossed: $" + gross + ")");
        queryStmt = "SELECT * " + 
                "FROM Actor NATURAL JOIN Stars NATURAL JOIN Movie;";
        query = connection.prepareStatement(queryStmt);
        rs = query.executeQuery();
        Stack<String> stars = new Stack<String>();
        while(rs.next()){
            stars.push(rs.getString("FirstName") + " " + rs.getString("LastName"));
        }
        System.out.println("Starring:\n");
        while(stars.peek() != null){
            System.out.println(stars.pop());
        }
        System.out.println();
    }

    public static void listTop() throws SQLException{
        System.out.print("How many of of the top earning movies would you like to display? ");
        int limit = sn.nextInt();
        String queryStmt =  "SELECT Title, Gross " +
                            "FROM Movie " +
                            "ORDER BY Gross ASC " +
                            "LIMIT ? ;";
        PreparedStatement query = connection.prepareStatement(queryStmt);
        query.setInt(1, limit);
        ResultSet rs = query.executeQuery();
        while(rs.next()){
            DecimalFormat formatter = new DecimalFormat("#,###.00");
            String gross = formatter.format(rs.getDouble("Gross"));
            System.out.println(rs.getString("Title") + "\t\t$" + gross);
            System.out.println();
        }
    }

    public static void addActor()throws SQLException{
        String name, alt;
        String[] names;
        System.out.print("Enter the first and last name of the actor: ");
        name = sn.next();
        name = name + " " + sn.next();
        System.out.print("Did this actor have another name they were credited under? If so, enter. Otherwise, enter \"no\" ");
        alt = sn.next();
        if(alt.equals("no")){
            alt = "";
        }else{
            while(sn.hasNext()){
                alt = alt + " " + sn.next();
            }
        }
        names = name.split(" ");
        String insertStmt = "INSERT INTO Actor (AID, FirstName, LastName, CreditAs) VALUES (? ? ? ?);";  
        PreparedStatement insert = connection.prepareStatement(insertStmt);
        insert.setString(2, names[0]);
        insert.setString(3, names[1]);
        insert.setString(4, alt);
        insert.executeUpdate();        
    }

    public static void addMovie() throws SQLException{
        Stack<String> actors = new Stack<String>();
        String insertStmt;
        boolean done = false;
        System.out.print("\nEnter the name of the movie: ");
        String title = sn.next();
        System.out.print("Enter its gross earnings: ");
        Double gross = sn.nextDouble();
        System.out.print("Enter the date released: ");
        String date = sn.next();
        while(!done){
            System.out.print("Enter the first and last name of an actor in the movie (enter \"done\" if finished): ");
            String first = sn.next();
            if(first.equals("done")){
                done = true;
            }else{
                String name = first + " " + sn.next();
                actors.push(name);
            }
        }
        if(actors.size() > 0){
            while(actors.peek() != null){
                String[] name = actors.pop().split(" ");
                insertStmt = "INSERT INTO Actor (AID, FirstName, LastName, CreditAs) VALUES (? ? ? ?);";
               	PreparedStatement insert = connection.prepareStatement(insertStmt);
                insert.setString(2, name[0]);
		        insert.setString(3, name[1]);
		        insert.setString(4, "");
		        insert.executeUpdate();
            }
        }
        insertStmt = "INSERT INTO Movie (Title, Gross, DateReleased) VALUES (? ? ?);";
       	PreparedStatement insert = connection.prepareStatement(insertStmt);
        insert.setString(1, title);
        insert.setDouble(2, gross);
        insert.setString(3, date);
        insert.executeUpdate();
    }

    public static Connection connect() throws IOException, SQLException{
        //read file for login
        Properties login = new Properties();
        try(FileReader in = new FileReader("login.properties")){
            login.load(in);
        }
        String usr = login.getProperty("username");
        String pass = login.getProperty("password"); 
        Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/MovieReview", usr, pass);
        return connection;
    }
}

