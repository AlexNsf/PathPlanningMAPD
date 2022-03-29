#include "map.h"

Map::Map()
{
    height = -1;
    width = -1;
    Grid = nullptr;
}

Map::~Map()
{
    if (Grid) {
        for (int i = 0; i < height; ++i)
            delete[] Grid[i];
        delete[] Grid;
    }
}

bool Map::get_map(const char *FileName)
{
    int rowiter = 0, grid_i = 0, grid_j = 0;

    tinyxml2::XMLElement *root = nullptr, *map = nullptr, *element = nullptr, *mapnode;

    std::string value;
    std::stringstream stream;

    bool hasGridMem = false, hasGrid = false, hasHeight = false, hasWidth = false, hasINIT,
    hasST = false, hasFIN = false, hasTASKS = false;

    tinyxml2::XMLDocument doc;

    // Load XML File
    if (doc.LoadFile(FileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cout << "Error opening XML file!" << std::endl;
        return false;
    }
    // Get ROOT element
    root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) {
        std::cout << "Error! No '" << CNS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }

    // Get MAP element
    map = root->FirstChildElement(CNS_TAG_MAP);
    if (!map) {
        std::cout << "Error! No '" << CNS_TAG_MAP << "' tag found in XML file!" << std::endl;
        return false;
    }

    for (mapnode = map->FirstChildElement(); mapnode; mapnode = mapnode->NextSiblingElement()) {
        element = mapnode->ToElement();
        value = mapnode->Value();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        stream.str("");
        stream.clear();

        if(value != CNS_TAG_GRID)
        {
            stream << element->GetText();
        }


        if (!hasGridMem && hasHeight && hasWidth) {
            Grid = new int *[height];
            for (int i = 0; i < height; ++i)
                Grid[i] = new int[width];
            hasGridMem = true;
        }

        if (value == CNS_TAG_HEIGHT) {
            if (hasHeight) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_HEIGHT << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_HEIGHT << "' =" << height << "will be used."
                          << std::endl;
            }
            else {
                if (!((stream >> height) && (height > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_HEIGHT
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_HEIGHT << "' tag should be an integer >=0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_HEIGHT
                              << "' tag will be encountered later..." << std::endl;
                }
                else
                    hasHeight = true;
            }
        }
        else if (value == CNS_TAG_WIDTH) {
            if (hasWidth) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_WIDTH << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_WIDTH << "' =" << width << "will be used." << std::endl;
            }
            else {
                if (!((stream >> width) && (width > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_WIDTH
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_WIDTH << "' tag should be an integer AND >0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_WIDTH
                              << "' tag will be encountered later..." << std::endl;

                }
                else
                    hasWidth = true;
            }
        }
        else if (value == CNS_TAG_INIT) {
            if (!hasWidth) {
                std::cout << "Error! '" << CNS_TAG_INIT << "' tag encountered before '" << CNS_TAG_WIDTH << "' tag."
                          << std::endl;
                return false;
            }
            if (!hasHeight) {
                std::cout << "Error! '" << CNS_TAG_INIT << "' tag encountered before '" << CNS_TAG_HEIGHT << "' tag."
                          << std::endl;
                return false;
            }

            if (hasINIT) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_INIT << "' encountered." << std::endl;
            }
            else {
                Coordinate cur_init;
                while (stream >> cur_init.j >> cur_init.i) {
                    if (cur_init.j < 0 || cur_init.j >= width) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_INIT
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_INIT << "' tag x coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_WIDTH << "' value, which is " << width << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_INIT
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    if (cur_init.i < 0 || cur_init.i >= height) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_INIT
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_INIT << "' tag y coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_HEIGHT << "' value, which is " << height << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_INIT
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    initial_locations.push_back(cur_init);
                }
                hasINIT = true;
            }
        }
        else if (value == CNS_TAG_ST) {
            if (!hasWidth) {
                std::cout << "Error! '" << CNS_TAG_ST << "' tag encountered before '" << CNS_TAG_WIDTH << "' tag."
                          << std::endl;
                return false;
            }
            if (!hasHeight) {
                std::cout << "Error! '" << CNS_TAG_ST << "' tag encountered before '" << CNS_TAG_HEIGHT << "' tag."
                          << std::endl;
                return false;
            }

            if (hasST) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_ST << "' encountered." << std::endl;
            }
            else {
                Coordinate cur_start;
                while (stream >> cur_start.j >> cur_start.i) {
                    if (cur_start.j < 0 || cur_start.j >= width) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_ST
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_ST << "' tag x coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_WIDTH << "' value, which is " << width << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_ST
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    if (cur_start.i < 0 || cur_start.i >= height) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_ST
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_ST << "' tag y coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_HEIGHT << "' value, which is " << height << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_ST
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    start_locations.push_back(cur_start);
                }
                hasST = true;
            }
        }
        else if (value == CNS_TAG_FIN) {
            if (!hasWidth) {
                std::cout << "Error! '" << CNS_TAG_FIN << "' tag encountered before '" << CNS_TAG_WIDTH << "' tag."
                          << std::endl;
                return false;
            }
            if (!hasHeight) {
                std::cout << "Error! '" << CNS_TAG_FIN << "' tag encountered before '" << CNS_TAG_HEIGHT << "' tag."
                          << std::endl;
                return false;
            }

            if (hasFIN) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_FIN << "' encountered." << std::endl;
            }
            else {
                Coordinate cur_finish;
                while (stream >> cur_finish.j >> cur_finish.i) {
                    if (cur_finish.j < 0 || cur_finish.j >= width) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_FIN
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_FIN << "' tag x coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_WIDTH << "' value, which is " << width << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_FIN
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    if (cur_finish.i < 0 || cur_finish.i >= height) {
                        std::cout << "Warning! Invalid value of '" << CNS_TAG_FIN
                                  << "' tag encountered (or could not convert to integer)" << std::endl;
                        std::cout << "Value of '" << CNS_TAG_FIN << "' tag y coordinate should be an integer AND >=0 AND < '"
                                  << CNS_TAG_HEIGHT << "' value, which is " << height << std::endl;
                        std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_FIN
                                  << "' tag will be encountered later..." << std::endl;
                    }
                    finish_locations.push_back(cur_finish);
                }
                hasFIN = true;
            }
        }
        else if (value == CNS_TAG_TASKS) {
            if (!hasST || !hasFIN) {
                std::cout << "Error! '" << CNS_TAG_TASKS << "' tag encountered before '" << CNS_TAG_ST << "' or '"
                          << CNS_TAG_FIN << "' tags."<< std::endl;
                return false;
            }
            if (hasTASKS) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_TASKS << "' encountered." << std::endl;
            }
            else {
                Task cur_task;
                size_t task_start;
                size_t task_finish;
                while (stream >> task_start >> task_finish) {
                    if (task_start < 0 || task_start >= start_locations.size()) {
                        std::cout << "Error! Value of '" << CNS_TAG_TASKS << "' tag start position number should be an integer AND >=0 AND < "
                                  << start_locations.size() << " - amount of start locations." << std::endl;
                        return false;
                    }
                    if (task_finish < 0 || task_finish >= finish_locations.size()) {
                        std::cout << "Error! Value of '" << CNS_TAG_TASKS << "' tag finish position number should be an integer AND >=0 AND < "
                                  << finish_locations.size() << " - amount of finish locations." << std::endl;
                        return false;
                    }
                    cur_task.start = start_locations[task_start];
                    cur_task.finish = finish_locations[task_finish];
                    tasks.push_back(cur_task);
                }
                hasTASKS = true;
            }
        }
        else if (value == CNS_TAG_GRID) {
            hasGrid = true;
            if (!(hasHeight && hasWidth)) {
                std::cout << "Error! No '" << CNS_TAG_WIDTH << "' tag or '" << CNS_TAG_HEIGHT << "' tag before '"
                          << CNS_TAG_GRID << "'tag encountered!" << std::endl;
                return false;
            }
            element = mapnode->FirstChildElement();
            while (grid_i < height) {
                if (!element) {
                    std::cout << "Error! Not enough '" << CNS_TAG_ROW << "' tags inside '" << CNS_TAG_GRID << "' tag."
                              << std::endl;
                    std::cout << "Number of '" << CNS_TAG_ROW
                              << "' tags should be equal (or greater) than the value of '" << CNS_TAG_HEIGHT
                              << "' tag which is " << height << std::endl;
                    return false;
                }
                std::string str = element->GetText();
                std::vector<std::string> elems;
                std::stringstream ss(str);
                std::string item;
                while (std::getline(ss, item, ' '))
                    elems.push_back(item);
                rowiter = grid_j = 0;
                int val;
                if (!elems.empty())
                    for (grid_j = 0; grid_j < width; ++grid_j) {
                        if (grid_j == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[grid_j];
                        stream >> val;
                        Grid[grid_i][grid_j] = val;
                    }

                if (grid_j != width) {
                    std::cout << "Invalid value on " << CNS_TAG_GRID << " in the " << grid_i + 1 << " " << CNS_TAG_ROW
                              << std::endl;
                    return false;
                }
                ++grid_i;

                element = element->NextSiblingElement();
            }
        }
    }
    //some additional checks
    if (!hasGrid) {
        std::cout << "Error! There is no tag 'grid' in xml-file!\n";
        return false;
    }

    if (!(hasINIT && hasST && hasFIN && hasTASKS))
        return false;

    for (const auto& init : initial_locations) {
        if (Grid[init.i][init.j]) {
            std::cout << "Error! Initial cell is not traversable (cell's value is" << Grid[init.i][init.j] << ")!"
                      << std::endl;
            return false;
        }
    }

    for (const auto& start : start_locations) {
        if (Grid[start.i][start.j]) {
            std::cout << "Error! Initial cell is not traversable (cell's value is" << Grid[start.i][start.j] << ")!"
                      << std::endl;
            return false;
        }
    }

    for (const auto& finish : initial_locations) {
        if (Grid[finish.i][finish.j]) {
            std::cout << "Error! Initial cell is not traversable (cell's value is" << Grid[finish.i][finish.j] << ")!"
                      << std::endl;
            return false;
        }
    }
    return true;
}

bool Map::CellIsTraversable(int i, int j) const
{
    return (Grid[i][j] == 0);
}

bool Map::CellIsObstacle(int i, int j) const
{
    return (Grid[i][j] != 0);
}

bool Map::CellOnGrid(int i, int j) const
{
    return (i < height && i >= 0 && j < width && j >= 0);
}

int Map::getValue(int i, int j) const
{
    if (i < 0 || i >= height)
        return -1;

    if (j < 0 || j >= width)
        return -1;

    return Grid[i][j];
}

int Map::getMapHeight() const
{
    return height;
}

int Map::getMapWidth() const
{
    return width;
}

const std::vector<Coordinate>& Map::get_initial_locations() const {
    return initial_locations;
}

const std::vector<Coordinate>& Map::get_start_locations() const {
    return start_locations;
}

const std::vector<Coordinate>& Map::get_finish_locations() const {
    return finish_locations;
}