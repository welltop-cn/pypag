#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <pag/pag.h>
#include "ffavc.h"

namespace pypag {
    bool surfaceReadPixels(std::shared_ptr<pag::PAGSurface> surface, pag::ColorType colorType, pag::AlphaType alphaType, py::buffer buf) {
        py::buffer_info info = buf.request();
        if (info.ndim != 1) {
            throw py::buffer_error();
        }

        int bufSize = info.itemsize * info.shape[0];
        if (bufSize < surface->width() * surface->height() * 4) {
            throw py::buffer_error();
        }
//        std::cout << info.itemsize << ", " << info.ndim 
//            << ", " << info.shape[0] << ", " << bufSize << std::endl;
        return surface->readPixels(colorType, alphaType, info.ptr, surface->width() * 4);
    }

    std::shared_ptr<pag::PAGImage> PAGImageFromBytes(py::buffer buf) {
        py::buffer_info info = buf.request();
        if (info.ndim != 1) {
            throw py::buffer_error();
        }

        int bufSize = info.itemsize * info.shape[0];
        return pag::PAGImage::FromBytes(info.ptr, bufSize);
    }

    std::shared_ptr<pag::PAGImage> PAGImageFromPixels(py::buffer buf, int width, int height, pag::ColorType colorType, pag::AlphaType alphaType) {
        py::buffer_info info = buf.request();
        if (info.ndim != 1) {
            throw py::buffer_error();
        }

        int bufSize = info.itemsize * info.shape[0];
 
        return pag::PAGImage::FromPixels(info.ptr, width, height, width * 4, colorType, alphaType);
    }

PYBIND11_MODULE(pypag, m) {
    //enums
    
    //ColorType
    py::enum_<pag::ColorType>(m, "ColorType")
        .value("Unknown", pag::ColorType::Unknown)
        .value("ALPHA_8", pag::ColorType::ALPHA_8)
        .value("RGBA_8888", pag::ColorType::RGBA_8888)
        .value("BGRA_8888", pag::ColorType::BGRA_8888);

    //AlphaType
    py::enum_<pag::AlphaType>(m, "AlphaType")
        .value("Unknown", pag::AlphaType::Unknown)
        .value("Opaque", pag::AlphaType::Opaque)
        .value("Premultiplied", pag::AlphaType::Premultiplied)
        .value("Unpremultiplied", pag::AlphaType::Unpremultiplied);

    py::class_<pag::PAGComposition, std::shared_ptr<pag::PAGComposition> >(m, "PAGComposition");

    py::class_<pag::PAGFont>(m, "PAGFont")
        .def_static("register_font", static_cast<pag::PAGFont (*)(const std::string&, int, const std::string&, const std::string&)>(&pag::PAGFont::RegisterFont), py::arg("font_path"), py::arg("ttc_index"), py::arg("font_family") = "", py::arg("font_style") = "")
        .def_readonly("font_family", &pag::PAGFont::fontFamily)
        .def_readonly("font_style", &pag::PAGFont::fontStyle);

    py::class_<pag::PAGFile, pag::PAGComposition, std::shared_ptr<pag::PAGFile> >(m, "PAGFile")
        .def_static("load", static_cast<std::shared_ptr<pag::PAGFile> (*)(const std::string&, const std::string&)>(&(pag::PAGFile::Load)), py::arg("file_path"), py::arg("password") = "")
        .def("num_images", &pag::PAGFile::numImages)
        .def("replace_images", &pag::PAGFile::replaceImage)
        .def("num_texts", &pag::PAGFile::numTexts)
        .def("get_text_data", &pag::PAGFile::getTextData)
        .def("replace_text", &pag::PAGFile::replaceText)
        .def_property_readonly("width", &pag::PAGFile::width)
        .def_property_readonly("height", &pag::PAGFile::height)
        .def_property("duration", &pag::PAGFile::duration, &pag::PAGFile::setDuration)
        .def_property_readonly("frame_rate", &pag::PAGFile::frameRate);



    py::class_<pag::PAGImage, std::shared_ptr<pag::PAGImage> >(m, "PAGImage")
        .def_static("from_path", &pag::PAGImage::FromPath)
        .def_static("from_bytes", &PAGImageFromBytes)
        .def_static("from_pixels", &PAGImageFromPixels)
        .def_property_readonly("width", &pag::PAGImage::width)
        .def_property_readonly("height", &pag::PAGImage::height);

    py::class_<pag::TextDocument, std::shared_ptr<pag::TextDocument> >(m, "TextDocument")
        .def_readwrite("text", &pag::TextDocument::text)
        .def_readwrite("font_family", &pag::TextDocument::fontFamily)
        .def_readwrite("font_style", &pag::TextDocument::fontStyle);

    py::class_<pag::PAGSurface, std::shared_ptr<pag::PAGSurface> >(m, "PAGSurface")
        .def_static("make_offscreen", &pag::PAGSurface::MakeOffscreen)
        .def("read_pixels", &surfaceReadPixels);

    py::class_<pag::PAGPlayer>(m, "PAGPlayer")
        .def(py::init<>())
        .def_property("surface", &pag::PAGPlayer::getSurface, &pag::PAGPlayer::setSurface)
        .def_property("composition", &pag::PAGPlayer::getComposition, &pag::PAGPlayer::setComposition)
        .def_property("progress", &pag::PAGPlayer::getProgress, &pag::PAGPlayer::setProgress)
        .def("flush", &pag::PAGPlayer::flush)
        .def("next_frame", &pag::PAGPlayer::nextFrame);
    
}

}//namespace pypag
