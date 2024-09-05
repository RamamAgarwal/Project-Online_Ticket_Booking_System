#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

// Enum for Language
enum class Language {
    HINDI,
    ENGLISH
};

// Enum for Genre
enum class Genre {
    ACTION,
    ROMANCE,
    COMEDY,
    HORROR
};

// Function to convert enum to string
std::string languageToString(Language language) {
    switch (language) {
        case Language::HINDI:
            return "HINDI";
        case Language::ENGLISH:
            return "ENGLISH";
        default:
            return "";
    }
}

std::string genreToString(Genre genre) {
    switch (genre) {
        case Genre::ACTION:
            return "ACTION";
        case Genre::ROMANCE:
            return "ROMANCE";
        case Genre::COMEDY:
            return "COMEDY";
        case Genre::HORROR:
            return "HORROR";
        default:
            return "";
    }
}

// Class for Movie
class Movie {
public:
    std::string name;
    float ratings;
    Language language;
    Genre genre;

    Movie(std::string name, Language language, Genre genre) : name(name), language(language), genre(genre), ratings(0.0f) {}

    std::string getName() const {
        return name;
    }

    float getRatings() const {
        return ratings;
    }

    Language getLanguage() const {
        return language;
    }

    Genre getGenre() const {
        return genre;
    }
};

// Class for Theater
class Theater {
public:
    static int idCounter;
    int id;
    std::string name;
    std::string location;
    int capacity;
    std::vector<class Show*> shows;

    Theater(std::string name, std::string location, int capacity) : name(name), location(location), capacity(capacity) {
        idCounter++;
        id = idCounter;
    }

    void updateShow(Show* oldShow, Show* newShow) {
        // To be implemented
    }

    std::vector<Show*> getShows() const {
        return shows;
    }

    std::string getName() const {
        return name;
    }

    int getCapacity() const {
        return capacity;
    }
};

int Theater::idCounter = 0;

// Class for Show
class Show {
public:
    static int idCounter;
    int id;
    std::chrono::system_clock::time_point showTime;
    Movie* movie;
    Theater* theater;
    int availableSeats;
    std::mutex mtx;

    Show(std::chrono::system_clock::time_point showTime, Movie* movie, Theater* theater) : showTime(showTime), movie(movie), theater(theater) {
        idCounter++;
        id = idCounter;
        availableSeats = theater->getCapacity();
        theater->shows.push_back(this);
    }

    Movie* getMovie() const {
        return movie;
    }

    void setTheater(Theater* theater) {
        this->theater = theater;
    }

    void setAvailableSeats(int availableSeats) {
        this->availableSeats = availableSeats;
    }

    int getAvailableSeats() const {
        return availableSeats;
    }

    class Ticket* bookTicket(class RegisteredUser* user, int seats) {
        std::lock_guard<std::mutex> lock(mtx);
        if (availableSeats >= seats && seats > 0) {
            Ticket* ticket = new Ticket();
            availableSeats -= seats;
            ticket->setOwner(user->getName());
            ticket->setBookedShow(this);
            ticket->setBookingTime(std::chrono::system_clock::now());
            ticket->setNumberOfSeats(seats);
            std::cout << "Successfully booked" << std::endl;
            user->bookingHistory.push_back(ticket);
            return ticket;
        } else {
            std::cout << "Seats not Available" << std::endl;
            return nullptr;
        }
    }

    std::string toString() const {
        return "Show{" +
               "id=" + std::to_string(id) +
               ", showTime=" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(showTime.time_since_epoch()).count()) +
               ", movie=" + movie->getName() +
               ", theater=" + theater->getName() +
               ", availableSeats=" + std::to_string(availableSeats) +
               '}';
    }
};

int Show::idCounter = 0;

// Class for Ticket
class Ticket {
public:
    static int idCounter;
    int id;
    std::string owner;
    std::chrono::system_clock::time_point bookingTime;
    int numberOfSeats;
    Show* bookedShow;

    Ticket() {
        idCounter++;
        id = idCounter;
       }

    std::string getOwner() const {
        return owner;
    }

    void setOwner(const std::string& owner) {
        this->owner = owner;
    }

    std::chrono::system_clock::time_point getBookingTime() const {
        return bookingTime;
    }

    void setBookingTime(const std::chrono::system_clock::time_point& bookingTime) {
        this->bookingTime = bookingTime;
    }

    int getNumberOfSeats() const {
        return numberOfSeats;
    }

    void setNumberOfSeats(int numberOfSeats) {
        this->numberOfSeats = numberOfSeats;
    }

    Show* getBookedShow() const {
        return bookedShow;
    }

    void setBookedShow(Show* bookedShow) {
        this->bookedShow = bookedShow;
    }

    std::string toString() const {
        return "Ticket{" +
               "owner='" + owner + '\'' +
               ", bookingTime=" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(bookingTime.time_since_epoch()).count()) +
               ", numberOfSeats=" + std::to_string(numberOfSeats) +
               ", bookedShow=" + bookedShow->toString() +
               '}';
    }
};

int Ticket::idCounter = 0;

// Abstract class for User
class User {
public:
    static int idCounter;
    int id;
    std::string name;

    User(std::string name) : name(name) {
        idCounter++;
        id = idCounter;
    }

    std::string getName() const {
        return name;
    }
};

int User::idCounter = 0;

// Class for GuestUser
class GuestUser : public User {
public:
    GuestUser(std::string name) : User(name) {}

    void registerUser(std::string username, std::string password) {
        // To be implemented
    }
};

// Class for RegisteredUser
class RegisteredUser : public User {
public:
    std::vector<Ticket*> bookingHistory;

    RegisteredUser(std::string name) : User(name) {}

    void cancelTicket(Ticket* ticket) {
        // To be implemented
    }
};

// Class for BookMyShow
class BookMyShow {
public:
    std::vector<Theater*> theaters;
    static std::map<std::string, std::vector<Show*>> movieMap;

    BookMyShow(std::vector<Theater*> theaters) : theaters(theaters) {
        generateMovieMap();
        std::cout << "BookMyShow initialized" << std::endl;
    }

    void generateMovieMap() {
        for (Theater* theater : theaters) {
            for (Show* show : theater->getShows()) {
                if (show != nullptr) {
                    if (movieMap.count(show->getMovie()->getName()) > 0) {
                        movieMap[show->getMovie()->getName()].push_back(show);
                    } else {
                        std::vector<Show*> movieShowList;
                        movieShowList.push_back(show);
                        movieMap[show->getMovie()->getName()] = movieShowList;
                    }
                }
            }
        }
    }

    static std::vector<Show*> searchShows(const std::string& movieName) {
        if (movieMap.count(movieName) > 0) {
            return movieMap[movieName];
        } else {
            return {};
        }
    }
};

std::map<std::string, std::vector<Show*>> BookMyShow::movieMap;

// Class for TicketBookingThread
class TicketBookingThread : public std::thread {
public:
    Show* show;
    RegisteredUser* user;
    int numberOfSeats;
    Ticket* ticket;

    TicketBookingThread(Show* show, RegisteredUser* user, int numberOfSeats) : show(show), user(user), numberOfSeats(numberOfSeats) {}

    void run() override {
        ticket = show->bookTicket(user, numberOfSeats);
    }

    Ticket* getTicket() const {
        return ticket;
    }
};

int main() {
    // Creating Guest User --> Piyush
    GuestUser piyush("Piyush");

    // Creating Registered User --> Ayush
    RegisteredUser ayush("Ayush");

    // Creating Registered User --> Saurabh
    RegisteredUser saurabh("Saurabh");

    // Creating Movie object --> Iron Man
    Movie ironMan("Iron Man", Language::ENGLISH, Genre::ACTION);

    // Creating Movie object --> Avengers: End Game
    Movie avengers("Avengers: End Game", Language::ENGLISH, Genre::ACTION);

    // Creating Movie object --> The Walk To Remember
    Movie walkToRemember("The Walk To Remember", Language::ENGLISH, Genre::ROMANCE);

    // Creating Movie object --> HouseFull2
    Movie housefull("HouseFull 2", Language::ENGLISH, Genre::COMEDY);

    / Creating Theater object --> PVR
    Theater pvr("PVR", "Delhi", 100);

    // Creating Theater object --> INOX
    Theater inox("INOX", "Mumbai", 150);

    // Creating Show object --> Iron Man at PVR
    Show ironManShow(std::chrono::system_clock::now() + std::chrono::hours(2), &ironMan, &pvr);

    // Creating Show object --> Avengers: End Game at INOX
    Show avengersShow(std::chrono::system_clock::now() + std::chrono::hours(4), &avengers, &inox);

    // Creating Show object --> The Walk To Remember at PVR
    Show walkToRememberShow(std::chrono::system_clock::now() + std::chrono::hours(6), &walkToRemember, &pvr);

    // Creating Show object --> HouseFull2 at INOX
    Show housefullShow(std::chrono::system_clock::now() + std::chrono::hours(8), &housefull, &inox);

    // Creating BookMyShow object
    BookMyShow bookMyShow({&pvr, &inox});

    // Searching for shows of Iron Man
    std::vector<Show*> ironManShows = BookMyShow::searchShows("Iron Man");

    // Booking ticket for Iron Man show at PVR
    TicketBookingThread thread1(&ironManShows[0], &ayush, 2);
    thread1.join();
    Ticket* ticket1 = thread1.getTicket();

    // Booking ticket for Avengers: End Game show at INOX
    TicketBookingThread thread2(&avengersShow, &saurabh, 3);
    thread2.join();
    Ticket* ticket2 = thread2.getTicket();

    return 0;
}