//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

#include <iostream>
#include <map>

namespace ex01 {
    class Article {
    public:
        Article(const std::string& name, double price) : name_(name), price_(price) {
            ;
        }

        std::string get_name() const {
            return name_;
        }

        double get_price() const {
            return price_;
        }
    private:
        std::string name_;
        double price_;
    };
    std::ostream& operator<<(std::ostream& s, const Article& a){
        s << a.get_name() << " $" << a.get_price();
        return s;
    }

    class Command {
    public:
        Command(unsigned id, const std::string& date, const std::string& client, std::map<Article*, int> articles):
        command_id_(id), date_(date), client_(client), articles_(articles) {
            ;
        }
        
        virtual ~Command() {
            articles_.clear();
        }

        const std::string get_client() const {
            return client_;
        }

        const std::string get_date() const {
            return date_;
        }

        unsigned get_id() const {
            return command_id_;
        }

        virtual double get_total_price() const {
            double total = 0;
            for (std::map<Article *, int>::const_iterator i = articles_.begin(); i != articles_.end(); ++i) {
                total += (i->second) * i->first->get_price();
            }
            return total;
        }

        void load_articles(std::ostream& s) const {
            for (std::map<Article *, int>::const_iterator i = articles_.begin(); i != articles_.end(); ++i) {
                s << "  - [ " << i->second << " | " << *(i->first) << " ]" << std::endl;
            }
        }        
    protected:
        unsigned command_id_;
        std::string date_;
        std::string client_;
        std::map<Article*, int> articles_;
    };
    std::ostream& operator<<(std::ostream& s, const Command& c) {
        s << "Command #" << c.get_id() << " by " << c.get_client() << " on " << c.get_date() << std::endl;
        s << "  List of Articles:" << std::endl;
        c.load_articles(s);
        s << "Total of $" << c.get_total_price();
        return s;
    }

    class ThuesdayDiscountCommand: public Command {
    public:
        ThuesdayDiscountCommand(unsigned id, const std::string& date, const std::string& client, std::map<Article*, int> articles): 
        Command(id, date, client, articles) {
            ;
        }
        double get_total_price() const {
            return Command::get_total_price() * 0.9; // Apply 10% disscount
        }
    };

    class PackageReductionDiscountCommand: public Command {
    public:
        PackageReductionDiscountCommand(unsigned id, const std::string& date, const std::string& client, std::map<Article*, int> articles): 
        Command(id, date, client, articles) {
            ;
        }
        double get_total_price() const {
            if (Command::get_total_price() > 150)
                return Command::get_total_price() - 10; // Apply -$10 disscount
            else
                return Command::get_total_price();
        }
    };
   
}
#endif
