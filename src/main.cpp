#include "ui/mainwindow.h"
#include "controller/artifact_controller.h"
#include "repository/csv_repository.h" // Include when ready
#include "repository/json_repository.h" // Include when ready
#include "repository/repository.h" // For the interface
#include <QApplication>
#include <vector> // For dummy repository
#include <memory> // For std::unique_ptr
#include <stdexcept> // For dummy repository find

// Dummy InMemoryRepository for initial UI testing
class InMemoryRepository : public Repository {
public:
    void addArtifact(const ArcheologicalArtifact& artifact) override {
        // Prevent duplicates by ID for this simple repo
        for (size_t i = 0; i < artifacts.size(); ++i) {
            if (artifacts[i].getId() == artifact.getId()) {
                artifacts[i] = artifact; // Update if ID exists
                return;
            }
        }
        artifacts.push_back(artifact);
    }
    void removeArtifact(const QString& artifactId) override {
        artifacts.erase(std::remove_if(artifacts.begin(), artifacts.end(),
                                       [&](const ArcheologicalArtifact& a){ return a.getId() == artifactId; }),
                        artifacts.end());
    }
    void updateArtifact(const ArcheologicalArtifact& artifact) override {
        for (auto& existingArtifact : artifacts) {
            if (existingArtifact.getId() == artifact.getId()) {
                existingArtifact = artifact;
                return;
            }
        }
        // Optionally, throw an exception or add if not found, depending on desired behavior
        // throw std::runtime_error("Artifact not found for update with ID: " + artifact.getId().toStdString());
        // Or, if update can mean "add if not exists":
        // artifacts.push_back(artifact);
    }
    ArcheologicalArtifact findArtifactById(const QString& artifactId) const override {
        for (const auto& artifact : artifacts) {
            if (artifact.getId() == artifactId) {
                return artifact;
            }
        }
        throw std::runtime_error("Artifact not found with ID: " + artifactId.toStdString()); // Or return an optional/default
    }
    std::vector<ArcheologicalArtifact> getAllArtifacts() const override {
        return artifacts;
    }
private:
    std::vector<ArcheologicalArtifact> artifacts;
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Create a repository instance (using CSV repository for persistence)
    std::unique_ptr<Repository> repo = std::make_unique<CsvRepository>("artifacts.csv");
    
    // Alternatively, you can use JSON repository:
    // std::unique_ptr<Repository> repo = std::make_unique<JsonRepository>("artifacts.json");
    
    // Or keep using InMemoryRepository for testing:
    // std::unique_ptr<Repository> repo = std::make_unique<InMemoryRepository>();

    // 2. Create the controller, passing ownership of the repository
    ArtifactController controller(std::move(repo));

    // 3. Create the MainWindow, passing a pointer to the controller
    MainWindow w(&controller);
    w.show();

    return a.exec();
}
