/*

qsettings - qsettings for shell scripts. Easy parse for INI files.

Copyright (c) 2019 Alfredo Pons Menargues  - apons@linucleus.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qsettings");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "qsettings for shell scripts. Easy parse for INI files."));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption keyOption(QStringList() << "k" << "key",QCoreApplication::translate("main", "Specify the key to search"), "machine/taxes");
    parser.addOption(keyOption);

    QCommandLineOption lineOption(QStringList() << "n" << "no-newline",QCoreApplication::translate("main", "do not output the trailing newline"));
    parser.addOption(lineOption);

    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Config INI file (qsettings format)."));
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (args.size() < 1) {
        std::cerr <<  qPrintable(QCoreApplication::translate("main", "Error: Must specify one filename argument.")) << std::endl;
        parser.showHelp(1);
    }

    QString file = args[args.size() - 1];

    QFileInfo check_file(file);
    if (!(check_file.exists() && check_file.isFile())) {
        qCritical() << "File: " << file << " not exist!";
        return EXIT_FAILURE;
    }

    QSettings settings (file, QSettings::IniFormat);

    if (parser.isSet(keyOption)) {
        QString key = parser.value(keyOption);
        if (key.isEmpty() || !settings.contains(key)) {
            std::cerr << "Key Not found!" << std::endl;
            return EXIT_FAILURE;
        }

       // qDebug() << "Key:  " << key;
        std::cout << settings.value(key).toString().toStdString();
        if (!parser.isSet(lineOption))
            std::cout <<  std::endl;


        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
