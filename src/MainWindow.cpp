// MainWindow.cpp - Main application window implementation

#include "MainWindow.h"
#include "BattleWidget.h"
#include "DatabaseManager.h"
#include "GameManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QStackedWidget>
#include <QWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stack_(nullptr),
      welcomePage_(nullptr),
      setupPage_(nullptr),
      profilePage_(nullptr),
      battlePage_(nullptr),
      gameOverPage_(nullptr),
      nameEdit_(nullptr),
      profileNameValue_(nullptr),
      profileLevelValue_(nullptr),
      profileExpValue_(nullptr),
      profileStateValue_(nullptr),
      scoreList_(nullptr),
      battleWidget_(nullptr),
      battleTitleLabel_(nullptr),
      gameOverTitleLabel_(nullptr),
      gameOverSummaryLabel_(nullptr),
      gameManager_(nullptr),
      databaseManager_(nullptr) {
    setWindowTitle("Battle Arena");
    setGeometry(100, 100, 800, 600);
    
    // Initialize managers
    databaseManager_ = new DatabaseManager("battlearena.db");
    databaseManager_->connect();
    gameManager_ = new GameManager();
    
    // Build the UI
    buildUi();
    
    // Connect game manager signals
    connect(gameManager_, &GameManager::battleFinished, this, &MainWindow::handleBattleFinished);
}

MainWindow::~MainWindow() {
    if (gameManager_) delete gameManager_;
    if (databaseManager_) delete databaseManager_;
}

void MainWindow::buildUi() {
    // Create stacked widget to hold all pages
    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    // Create the pages
    welcomePage_ = createWelcomePage();
    setupPage_ = createSetupPage();
    profilePage_ = createProfilePage();
    battlePage_ = createBattlePage();
    gameOverPage_ = createGameOverPage();

    // Add pages to stacked widget
    stack_->addWidget(welcomePage_);
    stack_->addWidget(setupPage_);
    stack_->addWidget(profilePage_);
    stack_->addWidget(battlePage_);
    stack_->addWidget(gameOverPage_);

    // Show welcome page initially
    showWelcomePage();
}

QWidget* MainWindow::createWelcomePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("Welcome to Battle Arena");
    QFont font = titleLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel("Test your skills in epic battles!");
    layout->addWidget(descLabel);

    layout->addSpacing(30);

    QPushButton *playButton = new QPushButton("Play Game");
    connect(playButton, &QPushButton::clicked, this, &MainWindow::showSetupPage);
    layout->addWidget(playButton);

    layout->addStretch();
    return page;
}

QWidget* MainWindow::createSetupPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *titleLabel = new QLabel("Character Setup");
    QFont font = titleLabel->font();
    font.setPointSize(18);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Enter your name:"));
    nameEdit_ = new QLineEdit();
    nameLayout->addWidget(nameEdit_);
    layout->addLayout(nameLayout);

    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Character Type:"));
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItem("Tank");
    typeCombo->addItem("Fast");
    typeCombo->addItem("Balanced");
    typeLayout->addWidget(typeCombo);
    layout->addLayout(typeLayout);

    QHBoxLayout *diffLayout = new QHBoxLayout();
    diffLayout->addWidget(new QLabel("Difficulty:"));
    QComboBox *diffCombo = new QComboBox();
    diffCombo->addItem("Easy");
    diffCombo->addItem("Normal");
    diffCombo->addItem("Hard");
    diffLayout->addWidget(diffCombo);
    layout->addLayout(diffLayout);

    layout->addSpacing(20);

    QPushButton *startButton = new QPushButton("Start Battle");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startDemo);
    layout->addWidget(startButton);

    layout->addStretch();
    return page;
}

QWidget* MainWindow::createProfilePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *titleLabel = new QLabel("Profile");
    QFont font = titleLabel->font();
    font.setPointSize(18);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    // Profile info grid
    QHBoxLayout *nameInfoLayout = new QHBoxLayout();
    nameInfoLayout->addWidget(new QLabel("Name:"));
    profileNameValue_ = new QLabel("-");
    nameInfoLayout->addWidget(profileNameValue_);
    nameInfoLayout->addStretch();
    layout->addLayout(nameInfoLayout);

    QHBoxLayout *levelInfoLayout = new QHBoxLayout();
    levelInfoLayout->addWidget(new QLabel("Level:"));
    profileLevelValue_ = new QLabel("1");
    levelInfoLayout->addWidget(profileLevelValue_);
    levelInfoLayout->addStretch();
    layout->addLayout(levelInfoLayout);

    QHBoxLayout *expInfoLayout = new QHBoxLayout();
    expInfoLayout->addWidget(new QLabel("Experience:"));
    profileExpValue_ = new QLabel("0");
    expInfoLayout->addWidget(profileExpValue_);
    expInfoLayout->addStretch();
    layout->addLayout(expInfoLayout);

    QHBoxLayout *stateInfoLayout = new QHBoxLayout();
    stateInfoLayout->addWidget(new QLabel("State:"));
    profileStateValue_ = new QLabel("Idle");
    stateInfoLayout->addWidget(profileStateValue_);
    stateInfoLayout->addStretch();
    layout->addLayout(stateInfoLayout);

    layout->addSpacing(20);

    QLabel *scoresLabel = new QLabel("High Scores:");
    layout->addWidget(scoresLabel);

    scoreList_ = new QListWidget();
    layout->addWidget(scoreList_);

    layout->addSpacing(10);

    QPushButton *backButton = new QPushButton("Back");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showWelcomePage);
    layout->addWidget(backButton);

    return page;
}

QWidget* MainWindow::createBattlePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    battleTitleLabel_ = new QLabel("Battle");
    QFont font = battleTitleLabel_->font();
    font.setPointSize(20);
    font.setBold(true);
    battleTitleLabel_->setFont(font);
    layout->addWidget(battleTitleLabel_);

    // Battle widget
    battleWidget_ = new BattleWidget();
    layout->addWidget(battleWidget_);

    layout->addSpacing(10);

    QPushButton *pauseButton = new QPushButton("Pause");
    layout->addWidget(pauseButton);

    return page;
}

QWidget* MainWindow::createGameOverPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    gameOverTitleLabel_ = new QLabel("Battle Complete!");
    QFont titleFont = gameOverTitleLabel_->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    gameOverTitleLabel_->setFont(titleFont);
    layout->addWidget(gameOverTitleLabel_);

    gameOverSummaryLabel_ = new QLabel("You have completed the battle!");
    layout->addWidget(gameOverSummaryLabel_);

    layout->addSpacing(20);

    QPushButton *restartButton = new QPushButton("Play Again");
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartDemo);
    layout->addWidget(restartButton);

    QPushButton *profileButton = new QPushButton("View Profile");
    connect(profileButton, &QPushButton::clicked, this, [this]() {
        refreshProfile();
        stack_->setCurrentWidget(profilePage_);
    });
    layout->addWidget(profileButton);

    layout->addStretch();
    return page;
}

void MainWindow::showWelcomePage() {
    stack_->setCurrentWidget(welcomePage_);
}

void MainWindow::showSetupPage() {
    if (nameEdit_) nameEdit_->clear();
    stack_->setCurrentWidget(setupPage_);
}

void MainWindow::startDemo() {
    if (!nameEdit_ || nameEdit_->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter your name");
        return;
    }

    // Start the game
    gameManager_->startGame(nameEdit_->text().toStdString());
    stack_->setCurrentWidget(battlePage_);
    refreshBattleView();
}

void MainWindow::handleBattleFinished() {
    showGameOverPage();
    if (!nameEdit_->text().isEmpty()) {
        databaseManager_->saveResult(nameEdit_->text().toStdString(), gameManager_->getCurrentScore());
    }
}

void MainWindow::restartDemo() {
    showSetupPage();
}

void MainWindow::refreshProfile() {
    if (!nameEdit_ || nameEdit_->text().isEmpty()) {
        return;
    }

    if (profileNameValue_) {
        profileNameValue_->setText(nameEdit_->text());
    }

    if (profileLevelValue_) {
        profileLevelValue_->setText(QString::number(gameManager_->getPlayerLevel()));
    }

    if (profileExpValue_) {
        profileExpValue_->setText(QString::number(gameManager_->getPlayerExp()));
    }

    if (profileStateValue_) {
        profileStateValue_->setText(stateTitle());
    }

    updateHighScores();
}

void MainWindow::refreshBattleView() {
    if (battleTitleLabel_) {
        battleTitleLabel_->setText(QString::fromStdString(gameManager_->getBattleTitle()));
    }

    if (battleWidget_) {
        battleWidget_->update();
    }
}

void MainWindow::showGameOverPage() {
    if (gameOverSummaryLabel_) {
        int finalScore = gameManager_->getCurrentScore();
        gameOverSummaryLabel_->setText(QString("Battle Score: %1").arg(finalScore));
    }
    stack_->setCurrentWidget(gameOverPage_);
}

void MainWindow::updateHighScores() {
    if (!scoreList_) return;

    scoreList_->clear();
    auto leaderboard = databaseManager_->getLeaderboard();
    for (const auto& score : leaderboard) {
        QString item = QString("%1 - Score: %2")
            .arg(QString::fromStdString(score.name))
            .arg(score.score);
        scoreList_->addItem(item);
    }
}

QString MainWindow::stateTitle() const {
    if (!gameManager_) return "Unknown";
    
    // Map game state to string
    // This would depend on GameManager implementation
    return "Idle";
}
