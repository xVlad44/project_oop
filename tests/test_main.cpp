#include <gtest/gtest.h>
#include "../src/domain/artifact.h"
#include "../src/repository/csv_repository.h"
#include "../src/repository/json_repository.h"
#include "../src/controller/artifact_controller.h"
#include "../src/controller/filter.h"
#include <QDate>
#include <QTemporaryFile>
#include <memory>

// Test fixture for Artifact tests
class ArtifactTest : public ::testing::Test {
protected:
    void SetUp() override {
        artifact = ArcheologicalArtifact("TEST001", "Test Artifact", "A test artifact", 
                                       "Bronze", QDate(2023, 5, 15), "Test Site");
    }
    
    ArcheologicalArtifact artifact;
};

// Test Artifact creation and getters
TEST_F(ArtifactTest, TestArtifactCreation) {
    EXPECT_EQ(artifact.getId(), "TEST001");
    EXPECT_EQ(artifact.getName(), "Test Artifact");
    EXPECT_EQ(artifact.getDescription(), "A test artifact");
    EXPECT_EQ(artifact.getMaterial(), "Bronze");
    EXPECT_EQ(artifact.getDiscoveryDate(), QDate(2023, 5, 15));
    EXPECT_EQ(artifact.getLocation(), "Test Site");
}

// Test Artifact setters
TEST_F(ArtifactTest, TestArtifactSetters) {
    artifact.setName("Modified Name");
    artifact.setMaterial("Iron");
    
    EXPECT_EQ(artifact.getName(), "Modified Name");
    EXPECT_EQ(artifact.getMaterial(), "Iron");
}

// Test fixture for Repository tests
class RepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        artifact1 = ArcheologicalArtifact("ID001", "Pottery Shard", "Ancient clay piece", 
                                        "Clay", QDate(2023, 1, 15), "Site A");
        artifact2 = ArcheologicalArtifact("ID002", "Arrowhead", "Flint arrowhead", 
                                        "Flint", QDate(2022, 5, 20), "Site B");
    }
    
    ArcheologicalArtifact artifact1;
    ArcheologicalArtifact artifact2;
};

// Test CSV Repository
TEST_F(RepositoryTest, TestCsvRepository) {
    QTemporaryFile tempFile;
    ASSERT_TRUE(tempFile.open());
    QString tempPath = tempFile.fileName();
    tempFile.close();
    
    {
        CsvRepository repo(tempPath);
        
        // Test adding artifacts
        EXPECT_NO_THROW(repo.addArtifact(artifact1));
        EXPECT_NO_THROW(repo.addArtifact(artifact2));
        
        // Test duplicate ID throws exception
        EXPECT_THROW(repo.addArtifact(artifact1), std::runtime_error);
        
        // Test retrieving artifacts
        auto retrieved = repo.findArtifactById("ID001");
        EXPECT_EQ(retrieved.getId(), artifact1.getId());
        EXPECT_EQ(retrieved.getName(), artifact1.getName());
        
        // Test getting all artifacts
        auto allArtifacts = repo.getAllArtifacts();
        EXPECT_EQ(allArtifacts.size(), 2);
        
        // Test updating artifact
        artifact1.setName("Updated Pottery");
        EXPECT_NO_THROW(repo.updateArtifact(artifact1));
        retrieved = repo.findArtifactById("ID001");
        EXPECT_EQ(retrieved.getName(), "Updated Pottery");
        
        // Test removing artifact
        EXPECT_NO_THROW(repo.removeArtifact("ID002"));
        EXPECT_THROW(repo.findArtifactById("ID002"), std::runtime_error);
        
        allArtifacts = repo.getAllArtifacts();
        EXPECT_EQ(allArtifacts.size(), 1);
    }
    
    // Test persistence - create new repository instance
    {
        CsvRepository repo2(tempPath);
        auto allArtifacts = repo2.getAllArtifacts();
        EXPECT_EQ(allArtifacts.size(), 1);
        EXPECT_EQ(allArtifacts[0].getName(), "Updated Pottery");
    }
}

// Test JSON Repository
TEST_F(RepositoryTest, TestJsonRepository) {
    QTemporaryFile tempFile;
    ASSERT_TRUE(tempFile.open());
    QString tempPath = tempFile.fileName();
    tempFile.close();
    
    {
        JsonRepository repo(tempPath);
        
        // Test adding artifacts
        EXPECT_NO_THROW(repo.addArtifact(artifact1));
        EXPECT_NO_THROW(repo.addArtifact(artifact2));
        
        // Test retrieving artifacts
        auto retrieved = repo.findArtifactById("ID001");
        EXPECT_EQ(retrieved.getId(), artifact1.getId());
        
        // Test getting all artifacts
        auto allArtifacts = repo.getAllArtifacts();
        EXPECT_EQ(allArtifacts.size(), 2);
    }
    
    // Test persistence
    {
        JsonRepository repo2(tempPath);
        auto allArtifacts = repo2.getAllArtifacts();
        EXPECT_EQ(allArtifacts.size(), 2);
    }
}

// Test fixture for Filter tests
class FilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        artifacts.push_back(ArcheologicalArtifact("ID001", "Bronze Sword", "Ancient bronze weapon", 
                                                "Bronze", QDate(1500, 1, 1), "Rome"));
        artifacts.push_back(ArcheologicalArtifact("ID002", "Clay Pot", "Cooking vessel", 
                                                "Clay", QDate(1200, 6, 15), "Athens"));
        artifacts.push_back(ArcheologicalArtifact("ID003", "Iron Spear", "Hunting weapon", 
                                                "Iron", QDate(800, 3, 10), "Sparta"));
        artifacts.push_back(ArcheologicalArtifact("ID004", "Bronze Shield", "Defensive equipment", 
                                                "Bronze", QDate(1300, 8, 20), "Rome"));
    }
    
    std::vector<ArcheologicalArtifact> artifacts;
};

// Test Name Filter
TEST_F(FilterTest, TestNameFilter) {
    NameFilter filter("Bronze", false);
    ArtifactFilter artifactFilter(std::make_unique<NameFilter>("Bronze", false));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Bronze Sword and Bronze Shield
    
    for (const auto& artifact : filtered) {
        EXPECT_TRUE(artifact.getName().contains("Bronze", Qt::CaseInsensitive));
    }
}

// Test Material Filter
TEST_F(FilterTest, TestMaterialFilter) {
    ArtifactFilter artifactFilter(std::make_unique<MaterialFilter>("Bronze", false));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Bronze Sword and Bronze Shield
    
    for (const auto& artifact : filtered) {
        EXPECT_EQ(artifact.getMaterial(), "Bronze");
    }
}

// Test Location Filter
TEST_F(FilterTest, TestLocationFilter) {
    ArtifactFilter artifactFilter(std::make_unique<LocationFilter>("Rome", false));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Bronze Sword and Bronze Shield from Rome
    
    for (const auto& artifact : filtered) {
        EXPECT_EQ(artifact.getLocation(), "Rome");
    }
}

// Test Date Range Filter
TEST_F(FilterTest, TestDateRangeFilter) {
    QDate startDate(1000, 1, 1);
    QDate endDate(1400, 12, 31);
    
    ArtifactFilter artifactFilter(std::make_unique<DateRangeFilter>(startDate, endDate));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Clay Pot and Bronze Shield
    
    for (const auto& artifact : filtered) {
        EXPECT_TRUE(artifact.getDiscoveryDate() >= startDate && 
                   artifact.getDiscoveryDate() <= endDate);
    }
}

// Test AND Filter
TEST_F(FilterTest, TestAndFilter) {
    auto andFilter = std::make_unique<AndFilter>();
    andFilter->addFilter(std::make_unique<MaterialFilter>("Bronze", false));
    andFilter->addFilter(std::make_unique<LocationFilter>("Rome", false));
    
    ArtifactFilter artifactFilter(std::move(andFilter));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Bronze Sword and Bronze Shield from Rome
    
    for (const auto& artifact : filtered) {
        EXPECT_EQ(artifact.getMaterial(), "Bronze");
        EXPECT_EQ(artifact.getLocation(), "Rome");
    }
}

// Test OR Filter
TEST_F(FilterTest, TestOrFilter) {
    auto orFilter = std::make_unique<OrFilter>();
    orFilter->addFilter(std::make_unique<MaterialFilter>("Clay", false));
    orFilter->addFilter(std::make_unique<MaterialFilter>("Iron", false));
    
    ArtifactFilter artifactFilter(std::move(orFilter));
    
    auto filtered = artifactFilter.filter(artifacts);
    EXPECT_EQ(filtered.size(), 2); // Clay Pot and Iron Spear
    
    for (const auto& artifact : filtered) {
        EXPECT_TRUE(artifact.getMaterial() == "Clay" || artifact.getMaterial() == "Iron");
    }
}

// Test fixture for Controller tests
class ControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary file for testing
        tempFile = std::make_unique<QTemporaryFile>();
        ASSERT_TRUE(tempFile->open());
        tempPath = tempFile->fileName();
        tempFile->close();
        
        // Create controller with CSV repository
        auto repo = std::make_unique<CsvRepository>(tempPath);
        controller = std::make_unique<ArtifactController>(std::move(repo));
        
        artifact1 = ArcheologicalArtifact("ID001", "Test Artifact 1", "Description 1", 
                                        "Material 1", QDate(2023, 1, 15), "Location 1");
        artifact2 = ArcheologicalArtifact("ID002", "Test Artifact 2", "Description 2", 
                                        "Material 2", QDate(2023, 2, 20), "Location 2");
    }
    
    void TearDown() override {
        controller.reset();
        tempFile.reset();
    }
    
    std::unique_ptr<QTemporaryFile> tempFile;
    QString tempPath;
    std::unique_ptr<ArtifactController> controller;
    ArcheologicalArtifact artifact1;
    ArcheologicalArtifact artifact2;
};

// Test Controller Add/Remove/Update with Undo/Redo
TEST_F(ControllerTest, TestControllerOperationsWithUndoRedo) {
    // Initially no artifacts and no undo/redo
    EXPECT_EQ(controller->getAllArtifacts().size(), 0);
    EXPECT_FALSE(controller->canUndo());
    EXPECT_FALSE(controller->canRedo());
    
    // Add artifact
    EXPECT_NO_THROW(controller->addArtifact(artifact1.getId(), artifact1.getName(), 
                                          artifact1.getDescription(), artifact1.getMaterial(),
                                          artifact1.getDiscoveryDate(), artifact1.getLocation()));
    
    EXPECT_EQ(controller->getAllArtifacts().size(), 1);
    EXPECT_TRUE(controller->canUndo());
    EXPECT_FALSE(controller->canRedo());
    
    // Add second artifact
    EXPECT_NO_THROW(controller->addArtifact(artifact2.getId(), artifact2.getName(), 
                                          artifact2.getDescription(), artifact2.getMaterial(),
                                          artifact2.getDiscoveryDate(), artifact2.getLocation()));
    
    EXPECT_EQ(controller->getAllArtifacts().size(), 2);
    
    // Test undo
    controller->undo();
    EXPECT_EQ(controller->getAllArtifacts().size(), 1);
    EXPECT_TRUE(controller->canUndo());
    EXPECT_TRUE(controller->canRedo());
    
    // Test redo
    controller->redo();
    EXPECT_EQ(controller->getAllArtifacts().size(), 2);
    EXPECT_TRUE(controller->canUndo());
    EXPECT_FALSE(controller->canRedo());
    
    // Test remove with undo
    controller->removeArtifact("ID001");
    EXPECT_EQ(controller->getAllArtifacts().size(), 1);
    
    controller->undo();
    EXPECT_EQ(controller->getAllArtifacts().size(), 2);
    
    // Test update with undo
    controller->updateArtifact("ID001", "ID001", "Updated Name", "Updated Description",
                             "Updated Material", QDate(2023, 12, 25), "Updated Location");
    
    auto updated = controller->getArtifactById("ID001");
    EXPECT_EQ(updated.getName(), "Updated Name");
    
    controller->undo();
    auto original = controller->getArtifactById("ID001");
    EXPECT_EQ(original.getName(), artifact1.getName());
}

// Test Controller Filtering
TEST_F(ControllerTest, TestControllerFiltering) {
    // Add test artifacts
    controller->addArtifact(artifact1.getId(), artifact1.getName(), 
                          artifact1.getDescription(), artifact1.getMaterial(),
                          artifact1.getDiscoveryDate(), artifact1.getLocation());
    controller->addArtifact(artifact2.getId(), artifact2.getName(), 
                          artifact2.getDescription(), artifact2.getMaterial(),
                          artifact2.getDiscoveryDate(), artifact2.getLocation());
    
    // Test name filtering
    auto nameFiltered = controller->filterArtifactsByName("Test Artifact 1", false);
    EXPECT_EQ(nameFiltered.size(), 1);
    EXPECT_EQ(nameFiltered[0].getId(), "ID001");
    
    // Test material filtering
    auto materialFiltered = controller->filterArtifactsByMaterial("Material 1", false);
    EXPECT_EQ(materialFiltered.size(), 1);
    EXPECT_EQ(materialFiltered[0].getId(), "ID001");
    
    // Test location filtering
    auto locationFiltered = controller->filterArtifactsByLocation("Location 2", false);
    EXPECT_EQ(locationFiltered.size(), 1);
    EXPECT_EQ(locationFiltered[0].getId(), "ID002");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
