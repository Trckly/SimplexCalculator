#ifndef TRANSPORTATIONDIALOG_H
#define TRANSPORTATIONDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class TransportationDialog : public QDialog {
private:
    QSpinBox* rowSpinBox;
    QSpinBox* colSpinBox;

public:
    TransportationDialog(QWidget* parent = nullptr) : QDialog(parent) {
        // Create SpinBoxes
        rowSpinBox = new QSpinBox(this);
        colSpinBox = new QSpinBox(this);

        // Set range for SpinBoxes (optional)
        rowSpinBox->setRange(2, 100);
        colSpinBox->setRange(2, 100);

        // Create labels
        QLabel* rowCountLabel = new QLabel("Rows count (from):", this);
        QLabel* colCountLabel = new QLabel("Columns count (to):", this);

        // Create OK and Cancel buttons
        QPushButton* okButton = new QPushButton("OK", this);
        QPushButton* cancelButton = new QPushButton("Cancel", this);

        // Connect the buttons
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        // Layout
        QVBoxLayout* layout = new QVBoxLayout(this);

        QHBoxLayout* row1 = new QHBoxLayout();
        row1->addWidget(rowCountLabel);
        row1->addWidget(rowSpinBox);

        QHBoxLayout* row2 = new QHBoxLayout();
        row2->addWidget(colCountLabel);
        row2->addWidget(colSpinBox);

        QHBoxLayout* buttonRow = new QHBoxLayout();
        buttonRow->addWidget(okButton);
        buttonRow->addWidget(cancelButton);

        // Add widgets to layout
        layout->addLayout(row1);
        layout->addLayout(row2);
        layout->addLayout(buttonRow);

        setLayout(layout);
        setWindowTitle("Transportation Table Settup");
    }

    // Getter functions to access spin box values
    int getRows() const {
        return rowSpinBox->value();
    }

    int getCols() const {
        return colSpinBox->value();
    }
};

#endif // TRANSPORTATIONDIALOG_H
