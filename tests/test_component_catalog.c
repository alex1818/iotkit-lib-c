/*
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../src/component_catalog.h"

bool testListAllComponentCatalogs() {
    char *response = NULL;

    response = listAllComponentCatalogs();

    printf("Response Received :%s\n", response);

    return true;
}

bool testGetComponentCatalogDetails() {
    char *response = NULL;

    response = getComponentCatalogDetails("temperature.v1.0");

    printf("Response Received :%s\n", response);

    return true;
}

bool testCreateAnComponentCatalog() {
    char *response = NULL;

    ComponentCatalog *cmpObject = createComponentCatalogObject("actua13", "1.0", "actuator", "Number", "float", "Degrees Celsius", "timeSeries");
    addMinValue(cmpObject, -150.0f);
    addMaxValue(cmpObject, 150.0f);
    addCommandString(cmpObject, "on");

    addCommandParams(cmpObject, "my1", "1-5");
    addCommandParams(cmpObject, "my2", "1,2");
    addCommandParams(cmpObject, "my3", "5,6,7");

    response = createAnComponentCatalog(cmpObject);
    printf("Response Received :%s\n", response);

    return true;
}

bool testUpdateAnComponentCatalog() {
    char *response = NULL;

    ComponentCatalog *cmpObject = createComponentCatalogObject(NULL, NULL, "actuator", "Number", "integer", "Degrees Celsius2", "timeSeries");
    addMinValue(cmpObject, -150.0f);
    addMaxValue(cmpObject, 150.0f);
    addCommandString(cmpObject, "off");

    addCommandParams(cmpObject, "my12", "1-5");
    addCommandParams(cmpObject, "my22", "5-10");

    response = updateAnComponentCatalog(cmpObject, "actua13.v1.0");
    printf("Response Received :%s\n", response);

    return true;
}
