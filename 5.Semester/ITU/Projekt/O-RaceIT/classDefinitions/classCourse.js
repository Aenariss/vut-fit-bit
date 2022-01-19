/**
 * Objekt reprezentující jednu trať.
 * @author Vojtěch Malý [xmalyv03]
 */
class courseClass {
    constructor(name, controls) {
        this.name = name;
        this.controlsSequence = controls;
    }
}

module.exports.courseClass = courseClass;