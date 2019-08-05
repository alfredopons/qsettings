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


    QCommandLineOption vectorOption(QStringList() << "j" << "vector-key",QCoreApplication::translate("main", "Specify the key vector to searh"),"vector key");
    parser.addOption(vectorOption);

    QCommandLineOption vectorIndexOption(QStringList() << "i" << "vector-index",QCoreApplication::translate("main", "Specify the index in vector to searh"),"vector index");
    parser.addOption(vectorIndexOption);

    QCommandLineOption vectorSizeOption(QStringList() << "s" << "vector-size",QCoreApplication::translate("main", "Get size vector"));
    parser.addOption(vectorSizeOption);

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

    if (parser.isSet(vectorSizeOption)) {
        if (!parser.isSet(vectorOption)) {
            std::cerr << "I need a vector key: Option -j" << std::endl;
            return EXIT_FAILURE;
        }
        QString key = parser.value(vectorOption);
        if (key.isEmpty()) {
            std::cerr << "Key Not found!: " << key.toStdString() << std::endl;
            return EXIT_FAILURE;
        }

        QStringList groups = settings.childGroups();
        bool match = false;
        for (const auto &i:groups){
            if (i == key) {
                match=true;
                break;
            }
        }
        if (!match) {
            std::cerr << "Vector Key Not found!: " << key.toStdString() << std::endl;
            return EXIT_FAILURE;
        }
        int size = settings.beginReadArray(key);
        settings.endArray();
        std::cout << size;
        if (!parser.isSet(lineOption))
            std::cout <<  std::endl;

        return EXIT_SUCCESS;
    }

     if (parser.isSet(vectorOption)) {
          if (!parser.isSet(keyOption)) {
              std::cerr << "I need a Key: Option -k" << std::endl;
              return EXIT_FAILURE;
          }
          if (!parser.isSet(vectorIndexOption)) {
              std::cerr << "I need a index in vector: Option -i" << std::endl;
              return EXIT_FAILURE;
          }

          QString key2 = parser.value(keyOption);
          if (key2.isEmpty()) {
              std::cerr << "Key Not found!: "  << std::endl;
              return EXIT_FAILURE;
          }

          QString key = parser.value(vectorOption);
          if (key.isEmpty()) {
              std::cerr << "Vector Key Not found!: "  << std::endl;
              return EXIT_FAILURE;
          }

          QStringList groups = settings.childGroups();
          bool match = false;
          for (const auto &i:groups){
              if (i == key) {
                  match=true;
                  break;
              }
          }
          if (!match) {
              std::cerr << "Vector Key Not found!: " << key.toStdString() << std::endl;
              return EXIT_FAILURE;
          }

          QString index = parser.value(vectorIndexOption);
          if (index.isEmpty()) {
              std::cerr << "index Not found!: " << std::endl;
              return EXIT_FAILURE;
          }
          int num = index.toInt();
          int size = settings.beginReadArray(key);
          if (num >= size || num < 0) {
              std::cerr << "Invalid index " << num;
                  return EXIT_FAILURE;
          }
          settings.setArrayIndex(num);
          if (key2.isEmpty() || !settings.contains(key2)) {
              std::cerr << "Key " << key2.toStdString() << " Not found!" << std::endl;
              settings.endArray();
              return EXIT_FAILURE;
          }
          std::cout << settings.value(key2).toString().toStdString();
          settings.endArray();
          if (!parser.isSet(lineOption))
              std::cout <<  std::endl;

          return EXIT_SUCCESS;
     }

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
