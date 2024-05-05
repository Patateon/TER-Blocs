#include "../headers/mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    openNDPButton = new QPushButton("Open", this);
    saveNDPButton = new QPushButton("Save", this);
    deleteButton = new QPushButton("Delete", this);
    clearButton = new QPushButton("Reduce", this);
    switchButton = new QPushButton("Switch", this);
    parseButton = new QPushButton("Parse", this);
    analyseButton = new QPushButton("Analyse (not working)", this);
    compareNDPButton = new QPushButton("Compare", this);
    openGLWidget = new OpenGLWindowWidget(this);

    openNDPLabel = new QLabel("Ouvre un fichier PLY contenant un nuage de point", this);
    saveNDPLabel = new QLabel("Sauvegarde le nuage de point actuel", this);
    deleteLabel = new QLabel("Supprime le nuage de point actuel", this);
    clearLabel = new QLabel("Réduit le nombre de point du NDP (en enlevant les points appartenants au mur)", this);
    switchLabel = new QLabel("Switch to the next point cloud", this);
    parseLabel = new QLabel("Parse le nuage de point actuel (kmean : prend du temps)", this);
    analyseLabel = new QLabel("Analyse le type de prise du NDP actuel (not working yet)", this);
    compareNDPLabel = new QLabel("Compare avec le nuage de point avant Reduce/Parsing", this);

    // Set word wrap property to true = elargie pas le coté gauche
    openNDPLabel->setWordWrap(true);
    saveNDPLabel->setWordWrap(true);
    deleteLabel->setWordWrap(true);
    clearLabel->setWordWrap(true);
    switchLabel->setWordWrap(true);
    parseLabel->setWordWrap(true);
    analyseLabel->setWordWrap(true);
    compareNDPLabel->setWordWrap(true);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(openNDPLabel);
    buttonLayout->addWidget(openNDPButton);
    buttonLayout->addWidget(compareNDPLabel);
    buttonLayout->addWidget(compareNDPButton);
    buttonLayout->addWidget(saveNDPLabel);
    buttonLayout->addWidget(saveNDPButton);
    buttonLayout->addWidget(deleteLabel);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(clearLabel);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(parseLabel);
    buttonLayout->addWidget(parseButton);
    buttonLayout->addWidget(switchLabel);
    buttonLayout->addWidget(switchButton);
    buttonLayout->addWidget(analyseLabel);
    buttonLayout->addWidget(analyseButton);
    buttonLayout->addStretch(); // Add stretch to push buttons to the top
    mainLayout->addLayout(buttonLayout, 1); // Buttons occupy 1/6 of the width

    mainLayout->addWidget(openGLWidget, 5); // OpenGL widget occupies 5/6 of the width
    setLayout(mainLayout);

    resize(1280, 720);
    connect(openNDPButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleOpenNDPButtonClicked);
    connect(saveNDPButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleSaveNDPButtonClicked);
    connect(deleteButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleDeleteCurrentNuageDePoint);
    connect(clearButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleClearCurrentNuageDePoint);
    connect(switchButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleSwitchNuageDePoint);
    connect(parseButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleParseNuageDePoint);
    connect(analyseButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleAnalyseNuageDePoint);
    connect(compareNDPButton, &QPushButton::clicked, openGLWidget, &OpenGLWindowWidget::handleCompareNuageDePoint);
    connect(openGLWidget, &OpenGLWindowWidget::compareButtonColorChanged, this, [this](bool green) {
        if (green) {
            compareNDPButton->setStyleSheet("background-color: green;");
        } else {
            // Set the button color to its default color
            compareNDPButton->setStyleSheet(""); // Set an empty style sheet to reset to default
        }
    });
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    openGLWidget->keyPressEvent(event);
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    openGLWidget->keyReleaseEvent(event);
}


MainWidget::~MainWidget()
{
    // Delete all buttons
    delete openNDPButton;
    delete saveNDPButton;
    delete deleteButton;
    delete clearButton;
    delete switchButton;
    delete parseButton;
    delete analyseButton;
    delete compareNDPButton;

    delete openNDPLabel;
    delete saveNDPLabel;
    delete deleteLabel;
    delete clearLabel;
    delete switchLabel;
    delete parseLabel;
    delete analyseLabel;
    delete compareNDPLabel;
}
