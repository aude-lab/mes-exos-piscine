const { Sequelize, DataTypes } = require("sequelize");

let sequelize;

async function connectToDB() {
    if (sequelize) {
        return sequelize;
    }

    sequelize = new Sequelize(
        "postgres",
        process.env.USERNAME,
        process.env.PASSWORD,
        {
            host: "/tmp",
            dialect: "postgres",
            port: 5432,
            logging: false,
        },
    );

    sequelize.define(
        "epitinder_users",
        {
            name: { type: DataTypes.STRING },
            age: { type: DataTypes.INTEGER },
            description: { type: DataTypes.STRING },
        },
        {
            timestamps: false,
            tableName: "epitinder_users",
        },
    );

    await sequelize.sync();
    return sequelize;
}

async function getAllUsers() {
    const sequelize = await connectToDB();

    return await sequelize.models.epitinder_users.findAll({
        raw: true,
    });
}

async function getUser(id) {
    if (!id || isNaN(id)) {
        return null;
    }

    const sequelize = await connectToDB();
    const user = await sequelize.models.epitinder_users.findByPk(id, {
        raw: true,
    });

    return user;
}

async function addUser(newUser) {
    if (
        !newUser ||
        newUser.id ||
        !newUser.name ||
        !newUser.age ||
        !newUser.description
    ) {
        return null;
    }

    const hasName = "name" in newUser && typeof newUser.name === "string";
    const hasAge = "age" in newUser && typeof newUser.age === "number";
    const hasDescription =
        "description" in newUser && typeof newUser.description === "string";

    const propertyCount = Object.keys(newUser).length;

    if (!hasName || !hasAge || !hasDescription || propertyCount !== 3) {
        return null;
    }

    const sequelize = await connectToDB();
    const user = await sequelize.models.epitinder_users.create(newUser);

    return user.get({ plain: true });
}

async function updateUser(user) {
    if (!user || !user.id || !user.name || !user.age || !user.description) {
        return null;
    }

    const hasName = "name" in user && typeof user.name === "string";
    const hasAge = "age" in user && typeof user.age === "number";
    const hasDescription =
        "description" in user && typeof user.description === "string";

    const propertyCount = Object.keys(user).length;

    if (!hasName || !hasAge || !hasDescription || propertyCount !== 4) {
        return null;
    }

    const { epitinder_users } = sequelize.models;
    const [updated] = await epitinder_users.update(user, {
        where: { id: user.id },
        returning: true,
    });

    return updated ? user : null;
}

async function deleteUser(id) {
    if (!id || isNaN(id)) {
        return null;
    }

    const sequelize = await connectToDB();
    const user = await sequelize.models.epitinder_users.findByPk(id, {
        raw: true,
    });

    if (!user) {
        return null;
    }

    await sequelize.models.epitinder_users.destroy({ where: { id } });
    return user;
}

async function getAllUsersName() {
    const sequelize = await connectToDB();

    return await sequelize.models.epitinder_users.findAll({
        attributes: ["name"],
        raw: true,
    });
}

async function getAllYoungAdults() {
    const sequelize = await connectToDB();

    return await sequelize.models.epitinder_users.findAll({
        where: {
            age: { [Sequelize.Op.gte]: 18, [Sequelize.Op.lt]: 30 },
        },
        raw: true,
    });

    /*return users.map((user) => user.get({ plain: true }));*/
}

module.exports = {
    connectToDB,
    getAllUsers,
    getUser,
    addUser,
    updateUser,
    deleteUser,
    getAllUsersName,
    getAllYoungAdults,
};
