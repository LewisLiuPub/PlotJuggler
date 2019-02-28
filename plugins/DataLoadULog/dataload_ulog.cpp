#include "dataload_ulog.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QProgressDialog>
#include "selectlistdialog.h"
#include "ulog_parser.h"

DataLoadULog::DataLoadULog()
{

}

const std::vector<const char*> &DataLoadULog::compatibleFileExtensions() const
{
    static  std::vector<const char*> extensions = { "ulg" };
    return extensions;
}


PlotDataMapRef DataLoadULog::readDataFromFile(const QString &file_name, bool)
{
    PlotDataMapRef plot_data;
    ULogParser parser( file_name.toStdString() );

    const auto& timeseries_map = parser.getTimeseriesMap();

    for( const auto& it: timeseries_map)
    {
        const std::string& sucsctiption_name =  it.first;
        const ULogParser::Timeseries& timeseries = it.second;

        for (const auto& data: timeseries.data )
        {
            std::string series_name = sucsctiption_name + data.first;

            auto series = plot_data.addNumeric( series_name );

            for( size_t i=0; i < data.second.size(); i++ )
            {
                double msg_time = static_cast<double>(timeseries.timestamps[i]) * 0.000001;
                PlotData::Point point( msg_time, data.second[i] );
                series->second.pushBack( point );
            }
        }
    }
    return plot_data;
}

DataLoadULog::~DataLoadULog()
{

}

QDomElement DataLoadULog::xmlSaveState(QDomDocument &doc) const
{
    QDomElement elem = doc.createElement("no_params");
    return elem;
}

bool DataLoadULog::xmlLoadState(QDomElement&)
{
    return true;
}
