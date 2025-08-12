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
#include <string>
using namespace std;

class GUI : public QWidget
{
    Q_OBJECT

private:
    Service serv = Service();
    QWidget* window = nullptr;
    QScrollArea* scrollArea = nullptr;

public:
    GUI(QWidget* parent = nullptr) : QWidget(parent) {
    }


public slots:
    void question() {
        window = new QWidget();

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
			
        connect(button1, &QPushButton::clicked, this, &GUI::main_menu);
        connect(button2, &QPushButton::clicked, this, &GUI::main_menu);
        connect(button3, &QPushButton::clicked, this, &GUI::main_menu);

        window->setLayout(Mainlayout);
        window->show();
    }
    void main_menu() {
        window->close();
        window = new QWidget();

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

        QLabel* label = new QLabel("Choose an option");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("Add dog");
        QPushButton* button2 = new QPushButton("Remove dog");
        QPushButton* button3 = new QPushButton("Update dog");
        QPushButton* button4 = new QPushButton("View dogs");
        QPushButton* button5 = new QPushButton("Log out");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        Mainlayout->addWidget(button1);
        Mainlayout->addWidget(button2);
        Mainlayout->addWidget(button3);
        Mainlayout->addWidget(button4);
        Mainlayout->addWidget(button5);

        connect(button1, &QPushButton::clicked, this, &GUI::adm_op1);
        connect(button2, &QPushButton::clicked, this, &GUI::adm_op2);
        connect(button3, &QPushButton::clicked, this, &GUI::adm_op3);
        connect(button4, &QPushButton::clicked, this, &GUI::adm_op4);
        connect(button5, &QPushButton::clicked, this, &GUI::main_menu);

        window->setLayout(Mainlayout);
        window->show();  
    }
    void adm_op1() {
        window->close();
        window = new QWidget();

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

        connect(button1, &QPushButton::clicked, this, &GUI::admin_menu);
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
		window->show();
    }
    void adm_op2() {
        window->close();
        window = new QWidget();

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

        connect(button1, &QPushButton::clicked, this, &GUI::admin_menu);
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void adm_op3() {
        window->close();
        window = new QWidget();

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

        connect(button1, &QPushButton::clicked, this, &GUI::admin_menu);
        connect(button2, &QPushButton::clicked, this, &GUI::admin_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void adm_op4() {
        window->close();
        window = new QWidget();
        window->setMinimumWidth(400);

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



    void user_menu() {
        window->close();
        if (scrollArea != nullptr)
            scrollArea->close();
        window = new QWidget();

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

        connect(button1, &QPushButton::clicked, this, &GUI::user_op1);
        connect(button2, &QPushButton::clicked, this, &GUI::user_op2);
        connect(button3, &QPushButton::clicked, this, &GUI::user_op3);
        connect(button4, &QPushButton::clicked, this, &GUI::user_op4);
        connect(button5, &QPushButton::clicked, this, &GUI::main_menu);

        window->setLayout(Mainlayout);
        window->show();
    }
    void user_op1() {
        window->close();
        window = new QWidget();

        QLabel* label = new QLabel("Dogs");
        label->setAlignment(Qt::AlignCenter);
        QPushButton* button1 = new QPushButton("Adopt");
        QPushButton* button2 = new QPushButton("Next");
        QPushButton* button3 = new QPushButton("Exit");

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);

        Mainlayout->addLayout(layout);

        connect(button3, &QPushButton::clicked, this, &GUI::user_menu);

        window->setLayout(Mainlayout);
        window->show();
    }
    void user_op2() {
        window->close();
        window = new QWidget();

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

        connect(button1, &QPushButton::clicked, this, [=]() { found_dogs(0, "test"); });
        connect(button2, &QPushButton::clicked, this, &GUI::user_menu);

        window->setLayout(MainLayout);
        window->show();
    }
    void found_dogs(int age, string breed) {
        window->close();
        window = new QWidget();
        window->setMinimumWidth(400);

        QLabel* label = new QLabel("Found dogs");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout* Mainlayout = new QVBoxLayout();
        Mainlayout->addWidget(label);

        int cnt = 0;
        for (auto& dog : serv.getServiceDogs()) {
            if (dog.get_age() == age && dog.get_breed() == breed) {
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
    void user_op3() {
        window->close();
        window = new QWidget();
        window->setMinimumWidth(400);

        QLabel* label = new QLabel("Dogs");
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

		QLabel* label = new QLabel("Adoption list has been opened in the corresponding app");
		label->setAlignment(Qt::AlignCenter);
		QPushButton* button = new QPushButton("Back");

		QVBoxLayout* Mainlayout = new QVBoxLayout();
		Mainlayout->addWidget(label);
		Mainlayout->addWidget(button);

		connect(button, &QPushButton::clicked, this, &GUI::user_menu);

		window->setLayout(Mainlayout);
		window->show();
    }
};
