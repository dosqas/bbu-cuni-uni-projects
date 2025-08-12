#include "service.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <string>
#include <QtCharts/qchart.h>
#include <QtCharts/qbarseries.h>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieSeries>
#include <qtooltip.h>
using namespace std;

class GUI : public QWidget
{
    Q_OBJECT

private:
    Service serv = Service();
    QWidget* window = nullptr;
    QScrollArea* scrollArea = nullptr;
    string save_mode;

public:
    GUI(QWidget* parent = nullptr) : QWidget(parent) {
    }


public slots:
    void question() {
        window = new QWidget();
        window->setWindowTitle("Save mode");

        QLabel* label = new QLabel("How should we save the adoption list?");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("CSV");
        QPushButton* button2 = new QPushButton("HTML");
        QPushButton* button3 = new QPushButton("No saving");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        QHBoxLayout* layout = new QHBoxLayout();

        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);
        Mainlayout->addLayout(layout);
			
        connect(button1, &QPushButton::clicked, this, [=]() { save_mode = "C"; serv.saveAdoptions(this->save_mode); main_menu(); });
        connect(button2, &QPushButton::clicked, this, [=]() { save_mode = "H"; serv.saveAdoptions(this->save_mode); main_menu(); });
        connect(button3, &QPushButton::clicked, this, [=]() { save_mode = "N"; main_menu(); });

        window->setLayout(Mainlayout);
        window->show();
    }
    void main_menu() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Main menu");

        QLabel* label = new QLabel("Choose user or shut down");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("Admin");
        QPushButton* button2 = new QPushButton("User");
        QPushButton* button3 = new QPushButton("Shut down");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);
        Mainlayout->addLayout(layout);

        connect(button1, &QPushButton::clicked, this, &GUI::admin_menu);
        connect(button2, &QPushButton::clicked, this, &GUI::user_menu);
        connect(button3, &QPushButton::clicked, window, &QWidget::close);

        window->setLayout(Mainlayout);
        window->show();
    }



    void admin_menu() {
        window->close();
        if (scrollArea != nullptr)
            scrollArea->close();
        window = new QWidget();
        window->setWindowTitle("Admin menu");

        QLabel* label = new QLabel("Choose an option");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("Add dog");
        QPushButton* button2 = new QPushButton("Remove dog");
        QPushButton* button3 = new QPushButton("Update dog");
        QPushButton* button4 = new QPushButton("View dogs");
        QPushButton* button5 = new QPushButton("Graphical data");
        QPushButton* button6 = new QPushButton("Log out");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        Mainlayout->addWidget(button1);
        Mainlayout->addWidget(button2);
        Mainlayout->addWidget(button3);
        Mainlayout->addWidget(button4);
        Mainlayout->addWidget(button5);
        Mainlayout->addWidget(button6);

        connect(button1, &QPushButton::clicked, this, &GUI::adm_op1);
        connect(button2, &QPushButton::clicked, this, &GUI::adm_op2);
        connect(button3, &QPushButton::clicked, this, &GUI::adm_op3);
        connect(button4, &QPushButton::clicked, this, &GUI::adm_op4);
        connect(button5, &QPushButton::clicked, this, &GUI::adm_op5);
        connect(button6, &QPushButton::clicked, this, &GUI::main_menu);

        window->setLayout(Mainlayout);
        window->show();  
    }
    void adm_op1() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Add dog");

        QLabel* label1 = new QLabel("Name");
        QLabel* label2 = new QLabel("Breed");
        QLabel* label3 = new QLabel("Age");
        QLabel* label4 = new QLabel("Photograph link");

        QLineEdit* line1 = new QLineEdit();
        QLineEdit* line2 = new QLineEdit();
        QLineEdit* line3 = new QLineEdit();
        QLineEdit* line4 = new QLineEdit();
        QPushButton* button1 = new QPushButton("Add");
        QPushButton* button2 = new QPushButton("Back");

        QFormLayout* MainLayout = new QFormLayout();
        QHBoxLayout* layout = new QHBoxLayout();

        MainLayout->addRow(label1, line1);
        MainLayout->addRow(label2, line2);
        MainLayout->addRow(label3, line3);
        MainLayout->addRow(label4, line4);
        layout->addWidget(button1);
        layout->addWidget(button2);

        MainLayout->addRow(layout);

        connect(button1, &QPushButton::clicked, this, [=]() { 
            if (line1->text().isEmpty() || line2->text().isEmpty() || line3->text().isEmpty() || line4->text().isEmpty()) {
				QMessageBox::warning(this, "Warning", "All fields must be filled!");
				return;
			}
            if (line3->text().toInt() == 0) {
				QMessageBox::warning(this, "Warning", "Age must be an integer!");
				return;
			}
            if (line3->text().toInt() < 0) {
                QMessageBox::warning(this, "Warning", "Age must be a positive integer!");
                return;
            }
            if (!serv.photoIsUnique(line4->text().toStdString())) {
				QMessageBox::warning(this, "Warning", "Photo link must be unique!");
				return;
			}
            serv.adminAddService(line1->text().toStdString(), line2->text().toStdString(), line3->text().toInt(), line4->text().toStdString()); 

            QMessageBox::information(this, "Success", "Dog added successfully!");
            });
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
		window->show();
    }
    void adm_op2() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Remove dog");

        QLabel* label = new QLabel("Photograph link");
        QLineEdit* line = new QLineEdit();
        QPushButton* button1 = new QPushButton("Remove");
        QPushButton* button2 = new QPushButton("Back");

        QFormLayout* MainLayout = new QFormLayout();
        QHBoxLayout* layout = new QHBoxLayout();

        MainLayout->addRow(label, line);
        layout->addWidget(button1);
        layout->addWidget(button2);

        MainLayout->addRow(layout);

        connect(button1, &QPushButton::clicked, this, [=]() {
            if (line->text().isEmpty()) {
				QMessageBox::warning(this, "Warning", "Photo link must be filled!");
				return;
            }
            if (serv.photoIsUnique(line->text().toStdString())) {
                QMessageBox::warning(this, "Warning", "Photo link is non-existent!");
                return;
            }
            if (!serv.isAdopted(line->text().toStdString())) {
				int response = QMessageBox::question(this, "Warning", "Dog is not adopted! Are you sure you want to remove it?", QMessageBox::Yes | QMessageBox::No);
                if (response == QMessageBox::No) {
					return;
				}
			}
            serv.adminRemoveService(line->text().toStdString());
            QMessageBox::information(this, "Success", "Dog removed successfully!");
            });
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void adm_op3() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Update dog");

        QLabel* label1 = new QLabel("Photograph link");
        QLabel* label2 = new QLabel("Name");
        QLabel* label3 = new QLabel("Breed");
        QLabel* label4 = new QLabel("Age");

        QLineEdit* line1 = new QLineEdit();
        QLineEdit* line2 = new QLineEdit();
        QLineEdit* line3 = new QLineEdit();
        QLineEdit* line4 = new QLineEdit();
        QPushButton* button1 = new QPushButton("Update");
        QPushButton* button2 = new QPushButton("Back");

        QFormLayout* MainLayout = new QFormLayout();
        QHBoxLayout* layout = new QHBoxLayout();

        MainLayout->addRow(label1, line1);
        MainLayout->addRow(label2, line2);
        MainLayout->addRow(label3, line3);
        MainLayout->addRow(label4, line4);
        layout->addWidget(button1);
        layout->addWidget(button2);

        MainLayout->addRow(layout);

        connect(button1, &QPushButton::clicked, this, [=]() {
            string name, breed;
            int age = 0;
            if (line1->text().isEmpty()) {
                QMessageBox::warning(this, "Warning", "Photo link must be filled!");
                return;
            }
            if (serv.photoIsUnique(line1->text().toStdString())) {
				QMessageBox::warning(this, "Warning", "Photo link is non-existent!");
				return;
			}
            for (auto& dog : serv.getServiceDogs()) {
                if (dog.get_photograph() == line1->text().toStdString()) {
                    if (!line2->text().isEmpty()) {
						name = line2->text().toStdString();
					}
                    else {
						name = dog.get_name();
					}
                    if (!line3->text().isEmpty()) {
						breed = line3->text().toStdString();
					}
                    else {
						breed = dog.get_breed();
					}
                    if (!line4->text().isEmpty()) {
                        if (line4->text().toInt() == 0) {
							QMessageBox::warning(this, "Warning", "Age must be an integer!");
							return;
						}
                        if (line4->text().toInt() < 0) {
							QMessageBox::warning(this, "Warning", "Age must be a positive integer!");
							return;
						}
						age = line4->text().toInt();
					}
                    else {
						age = dog.get_age();
					}
					break;
				}
			}
            serv.adminUpdateService(line1->text().toStdString(), name, breed, age);
            QMessageBox::information(this, "Success", "Dog updated successfully!");
            });
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void adm_op4() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("View dogs");
        window->setMinimumWidth(380);

        QLabel* label = new QLabel("Dogs");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);

        int cnt = 0;
        for (auto& dog : serv.getServiceDogs()) {
            cnt++;
            string s = dog.dog_to_string();
            s.pop_back();
            s.pop_back();
            QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
            Mainlayout->addWidget(label);
        }
        Mainlayout->setContentsMargins(10, 10, 10, 10);

        QPushButton* button = new QPushButton("Back");
        Mainlayout->addStretch(1);
        Mainlayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(Mainlayout);
        scrollArea = new QScrollArea();
        scrollArea->setMinimumWidth(400);
        scrollArea->setWidget(window);
        scrollArea->show();
    }
    void adm_op5() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Dog Data Distribution");
        window->setMinimumSize(800, 600);

        vector<class Dog>& dogs = serv.getServiceDogs();
        std::map<int, int> ageCounts;
        std::map<string, int> breedCounts;
        for (const Dog& dog : dogs) {
            ageCounts[dog.get_age()]++;
            breedCounts[dog.get_breed()]++;
        }

        QVector<double> ages, counts;
        for (const auto& pair : ageCounts) {
            ages.push_back(pair.first);
            counts.push_back(pair.second);
        }

        QBarSeries* series = new QBarSeries();

        for (int i = 0; i < ages.size(); ++i) {
            QBarSet* set = new QBarSet(QString::number(ages[i]));
            *set << counts[i];
            set->setLabel(QString::number(ages[i]));
            QObject::connect(set, &QBarSet::hovered, [counts, i](bool isHovered) {
                if (isHovered) {
                    QToolTip::showText(QCursor::pos(), QString::number(counts[i]));
                }
                });
            series->append(set);
        }


        QChart* chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Dog Age Distribution");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QChartView* view = new QChartView(chart);
        view->setMinimumSize(600, 400);

        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setAlignment(Qt::AlignCenter);
        scrollArea->setWidget(view);

        QPieSeries* pieSeries = new QPieSeries();

        for (const auto& pair : breedCounts) {
            QPieSlice* slice = new QPieSlice(QString::fromStdString(pair.first), pair.second);
            QObject::connect(slice, &QPieSlice::hovered, [pair](bool isHovered) {
                if (isHovered) {
                    QToolTip::showText(QCursor::pos(), QString::number(pair.second));
                }
                });
            pieSeries->append(slice);
        }


        QChart* pieChart = new QChart();
        pieChart->addSeries(pieSeries);
        pieChart->setTitle("Dog Breed Distribution");
        pieChart->setAnimationOptions(QChart::SeriesAnimations);

        QChartView* pieView = new QChartView(pieChart);
        pieView->setMinimumSize(600, 400);

        QScrollArea* pieScrollArea = new QScrollArea();
        pieScrollArea->setAlignment(Qt::AlignCenter);
        pieScrollArea->setWidget(pieView);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(scrollArea);
        layout->addWidget(pieScrollArea);

        QPushButton* button = new QPushButton("Back");
        layout->addWidget(button);

        window->setLayout(layout);
        window->show();

        connect(button, &QPushButton::clicked, this, &GUI::admin_menu);
    }



    void user_menu() {
        window->close();
        if (scrollArea != nullptr)
            scrollArea->close();
        window = new QWidget();
        window->setWindowTitle("User menu");

        QLabel* label = new QLabel("Choose an option");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("See dogs one by one");
        QPushButton* button2 = new QPushButton("See dogs of a given breed younger than an age");
        QPushButton* button3 = new QPushButton("See adoption list");
        QPushButton* button4 = new QPushButton("See adoption list using CSV/HTML");
        QPushButton* button5 = new QPushButton("Log out");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        Mainlayout->addWidget(button1);
        Mainlayout->addWidget(button2);
        Mainlayout->addWidget(button3);
        Mainlayout->addWidget(button4);
        Mainlayout->addWidget(button5);

        connect(button1, &QPushButton::clicked, this, [=] { user_op1(0); });
        connect(button2, &QPushButton::clicked, this, &GUI::user_op2);
        connect(button3, &QPushButton::clicked, this, &GUI::user_op3);
        connect(button4, &QPushButton::clicked, this, [=]() {
            if (save_mode == "N") {
				QMessageBox::warning(this, "Warning", "No adoption list save mode has been chosen!");
				return;
            }
            user_op4();
            });
        connect(button5, &QPushButton::clicked, this, &GUI::main_menu);

        window->setLayout(Mainlayout);
        window->show();
    }
    void user_op1(int cnt) {
        window->close();
        window = new QWidget();
        window->setWindowTitle("See dogs one by one");

        QLabel* label = new QLabel("Unadopted dogs");

        vector<class Dog>& dogs = serv.getServiceDogs();
        size_t nr_of_adoptions = dogs.size();

        nr_of_adoptions -= count_if(dogs.begin(), dogs.end(), [](const Dog& dog) {
            return dog.get_adopted();
            });

        if (nr_of_adoptions == 0) {
		    QMessageBox::warning(this, "Warning", "No unadopted dogs to show!");
            user_menu();
		}
        cnt = cnt % dogs.size();
        while (dogs[cnt].get_adopted()) {
            cnt++;
            cnt = cnt % dogs.size();
		}

        QLabel* dogLabel = new QLabel(QString::fromStdString(dogs[cnt].dog_to_string()));
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("Adopt");
        QPushButton* button2 = new QPushButton("Next");
        QPushButton* button3 = new QPushButton("Back");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        Mainlayout->addWidget(dogLabel);
        int dogLabelIndex = Mainlayout->indexOf(dogLabel);
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);

        Mainlayout->addLayout(layout);

        connect(button3, &QPushButton::clicked, this, &GUI::user_menu);
        connect(button2, &QPushButton::clicked, this, [=]() {
            user_op1(cnt + 1);
            });
        connect(button1, &QPushButton::clicked, this, [this, &dogs, cnt]() {
            dogs[cnt].set_adopted(true);
            serv.saveToFileServ();
            QMessageBox::information(this, "Success", "Dog adopted successfully!");
            user_op1(cnt + 1);
            });

        window->setLayout(Mainlayout);
        window->show();
    }
    void user_op2() {
        window->close();
        if (scrollArea != nullptr)
            scrollArea->close();
        window = new QWidget();
        window->setWindowTitle("See dogs of a given breed younger than an age");

        QLabel* label1 = new QLabel("Breed");
        QLabel* label2 = new QLabel("Age");
        QLineEdit* line1 = new QLineEdit();
        QLineEdit* line2 = new QLineEdit();
        QPushButton* button1 = new QPushButton("Find dogs");
        QPushButton* button2 = new QPushButton("Back");

        QFormLayout* MainLayout = new QFormLayout();
        QHBoxLayout* layout = new QHBoxLayout();

        MainLayout->addRow(label1, line1);
        MainLayout->addRow(label2, line2);
        layout->addWidget(button1);
        layout->addWidget(button2);

        MainLayout->addRow(layout);

        connect(button1, &QPushButton::clicked, this, [=]() {
            string breed = "";
            int age = -1;
            if (!line1->text().isEmpty()) {
                breed = line1->text().toStdString();
            }
            if (!line2->text().isEmpty()) {
                if (line2->text().toInt() == 0) {
					QMessageBox::warning(this, "Warning", "Age must be an integer!");
					return;
				}
                if (line2->text().toInt() < 1) {
					QMessageBox::warning(this, "Warning", "Age must be a non-zero, positive integer!");
					return;
				}
				age = line2->text().toInt();
			}
            found_dogs(age, breed); });
        connect(button2, &QPushButton::clicked, this, &GUI::user_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void found_dogs(int age, string breed) {
        window->close();
        window = new QWidget();
        window->setWindowTitle("Found dogs");
        window->setMinimumWidth(380);

        QLabel* label = new QLabel("Found dogs");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);

        int cnt = 0;
        for (auto& dog : serv.getServiceDogs()) {
            if (age == -1 && breed == "") {
				cnt++;
				string s = dog.dog_to_string();
				s.pop_back();
				s.pop_back();
				QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
				Mainlayout->addWidget(label);
			}
            else if (age == -1) {
                if (dog.get_breed() == breed) {
                    cnt++;
                    string s = dog.dog_to_string();
                    s.pop_back();
                    s.pop_back();
                    QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
                    Mainlayout->addWidget(label);
                }
            }
            else if (breed == "") {
                if (dog.get_age() < age) {
					cnt++;
					string s = dog.dog_to_string();
					s.pop_back();
					s.pop_back();
					QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
					Mainlayout->addWidget(label);
				}
			}
			else if (dog.get_age() < age && dog.get_breed() == breed) {
				cnt++;
				string s = dog.dog_to_string();
				s.pop_back();
				s.pop_back();
				QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
				Mainlayout->addWidget(label);
			}
        }
        Mainlayout->setContentsMargins(10, 10, 10, 10);

        QPushButton* button = new QPushButton("Back");
        Mainlayout->addStretch(1);
        Mainlayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, &GUI::user_op2);

        window->setLayout(Mainlayout);
        scrollArea = new QScrollArea();
        scrollArea->setMinimumWidth(400);
        scrollArea->setWidget(window);
        scrollArea->show();
    }
    void user_op3() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("See adoption list");
        window->setMinimumWidth(380);

        QLabel* label = new QLabel("Adopted dogs");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);

        int cnt = 0;
        for (auto& dog : serv.getServiceDogs()) {
            if (dog.get_adopted()) {
                cnt++;
                string s = dog.dog_to_string();
                s.pop_back();
                s.pop_back();
                QLabel* label = new QLabel(QString::fromStdString("[" + to_string(cnt) + "]  " + s));
                Mainlayout->addWidget(label);
            }
        }
        Mainlayout->setContentsMargins(10, 10, 10, 10);

        QPushButton* button = new QPushButton("Back");
        Mainlayout->addStretch(1);
        Mainlayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, &GUI::user_menu);

        window->setLayout(Mainlayout);
        scrollArea = new QScrollArea();
        scrollArea->setMinimumWidth(400);
        scrollArea->setWidget(window);
        scrollArea->show();
    }
    void user_op4() {
        window->close();
        window = new QWidget();
        window->setWindowTitle("See adoption list using CSV/HTML");

		QLabel* label = new QLabel("Adoption list has been opened in the corresponding app");
		label->setAlignment(Qt::AlignCenter);
		QPushButton* button = new QPushButton("Back");

		QVBoxLayout* Mainlayout = new QVBoxLayout();
		Mainlayout->addWidget(label);
		Mainlayout->addWidget(button);

		connect(button, &QPushButton::clicked, this, &GUI::user_menu);

		window->setLayout(Mainlayout);
		window->show();

        serv.loadFromType(save_mode);
    }
};
