#pragma once

#include <QQuickImageProvider>
#include <QPixmap>

class ImageProvider : public QQuickImageProvider
{
    public:
    ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;
};