/*
    cam2web - streaming camera to web

    Copyright (C) 2017, cvsandbox, cvsandbox@gmail.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <map>
#include <algorithm>
#include "XRaspiCameraConfig.hpp"

using namespace std;

const static string  PROP_HFLIP               = "hflip";
const static string  PROP_VFLIP               = "vflip";
const static string  PROP_VIDEO_STABILISATION = "videostabilisation";
const static string  PROP_SHARPNESS           = "sharpness";
const static string  PROP_CONTRAST            = "contrast";
const static string  PROP_BRIGHTNESS          = "brightness";
const static string  PROP_SATURATION          = "saturation";
const static string  PROP_AWBMODE             = "awb";
const static string  PROP_EXPMODE             = "expmode";
const static string  PROP_EXPMETERINGMODE     = "expmeteringmode";
const static string  PROP_EFFECT              = "effect";

const static string* SupportedProperties[] =
{
    &PROP_HFLIP, &PROP_VFLIP, &PROP_VIDEO_STABILISATION,
    &PROP_SHARPNESS, &PROP_CONTRAST, &PROP_BRIGHTNESS, &PROP_SATURATION,
    &PROP_AWBMODE, &PROP_EXPMODE, &PROP_EXPMETERINGMODE, &PROP_EFFECT
};

const static map<string, AwbMode> SupportedAwbModes =
{
    { "Off",          AwbMode::Off          },
    { "Auto",         AwbMode::Auto         },
    { "Sunlight",     AwbMode::Sunlight     },
    { "Cloudy",       AwbMode::Cloudy       },
    { "Shade",        AwbMode::Shade        },
    { "Tungsten",     AwbMode::Tungsten     },
    { "Fluorescent",  AwbMode::Fluorescent  },
    { "Incandescent", AwbMode::Incandescent },
    { "Flash",        AwbMode::Flash        },
    { "Horizon",      AwbMode::Horizon      }
};

const static map<string, ExposureMode> SupportedExposureModes =
{
    { "Off",          ExposureMode::Off          },
    { "Auto",         ExposureMode::Auto         },
    { "Night",        ExposureMode::Night        },
    { "NightPreview", ExposureMode::NightPreview },
    { "Backlight",    ExposureMode::Backlight    },
    { "Spotlight",    ExposureMode::Spotlight    },
    { "Sports",       ExposureMode::Sports       },
    { "Snow",         ExposureMode::Snow         },
    { "Beach",        ExposureMode::Beach        },
    { "VeryLong",     ExposureMode::VeryLong     },
    { "FixedFps",     ExposureMode::FixedFps     },
    { "AntiShake",    ExposureMode::AntiShake    },
    { "FireWorks",    ExposureMode::FireWorks    }
};

const static map<string, ExposureMeteringMode> SupportedExposureMeteringModes =
{
    { "Average", ExposureMeteringMode::Average },
    { "Spot",    ExposureMeteringMode::Spot    },
    { "Backlit", ExposureMeteringMode::Backlit },
    { "Matrix",  ExposureMeteringMode::Matrix  }
};

const static map<string, ImageEffect> SupportedImageEffects =
{
    { "None",         ImageEffect::None         },
    { "Negative",     ImageEffect::Negative     },
    { "Solarize",     ImageEffect::Solarize     },
    { "Sketch",       ImageEffect::Sketch       },
    { "Denoise",      ImageEffect::Denoise      },
    { "Emboss",       ImageEffect::Emboss       },
    { "OilPaint",     ImageEffect::OilPaint     },
    { "Hatch",        ImageEffect::Hatch        },
    { "Gpen",         ImageEffect::Gpen         },
    { "Pastel",       ImageEffect::Pastel       },
    { "WaterColor",   ImageEffect::WaterColor   },
    { "Film",         ImageEffect::Film         },
    { "Blur",         ImageEffect::Blur         },
    { "Saturation",   ImageEffect::Saturation   },
    { "ColorSwap",    ImageEffect::ColorSwap    },
    { "WashedOut",    ImageEffect::WashedOut    },
    { "Posterise",    ImageEffect::Posterise    },
    { "ColorPoint",   ImageEffect::ColorPoint   },
    { "ColorBalance", ImageEffect::ColorBalance },
    { "Cartoon",      ImageEffect::Cartoon      }
};

XRaspiCameraConfig::XRaspiCameraConfig( const shared_ptr<XRaspiCamera>& camera ) :
    mCamera( camera )
{

}

// Set the specified property of a PI camera
XError XRaspiCameraConfig::SetProperty( const string& propertyName, const string& value )
{
    XError ret = XError::Success;
    int    numericValue;
    bool   propStatus = true;

    // many configuration setting are actually numeric, so scan it once
    int scannedCount = sscanf( value.c_str( ), "%d", &numericValue );

    if ( propertyName == PROP_HFLIP )
    {
        bool hflip = ( ( value == "1" ) || ( value == "true" ) );
        bool vflip = mCamera->GetVerticalFlip( );

        propStatus = mCamera->SetCameraFlip( hflip, vflip );
    }
    else if ( propertyName == PROP_VFLIP )
    {
        bool vflip = ( ( value == "1" ) || ( value == "true" ) );
        bool hflip = mCamera->GetHorizontalFlip( );

        propStatus = mCamera->SetCameraFlip( hflip, vflip );
    }
    else if ( propertyName == PROP_VIDEO_STABILISATION )
    {
        propStatus = mCamera->SetVideoStabilisation( ( value == "1" ) || ( value == "true" ) );
    }
    else if ( propertyName == PROP_SHARPNESS )
    {
        if ( scannedCount == 1 )
        {
            propStatus = mCamera->SetSharpness( numericValue );
        }
        else
        {
            ret = XError::InvalidPropertyValue;
        }
    }
    else if ( propertyName == PROP_CONTRAST )
    {
        if ( scannedCount == 1 )
        {
            propStatus = mCamera->SetContrast( numericValue );
        }
        else
        {
            ret = XError::InvalidPropertyValue;
        }
    }
    else if ( propertyName == PROP_BRIGHTNESS )
    {
        if ( scannedCount == 1 )
        {
            propStatus = mCamera->SetBrightness( numericValue );
        }
        else
        {
            ret = XError::InvalidPropertyValue;
        }    
    }
    else if ( propertyName == PROP_SATURATION )
    {
        if ( scannedCount == 1 )
        {
            propStatus = mCamera->SetSaturation( numericValue );
        }
        else
        {
            ret = XError::InvalidPropertyValue;
        }
    }
    else if ( propertyName == PROP_AWBMODE )
    {
        map<string, AwbMode>::const_iterator itMode = SupportedAwbModes.find( value );
        
        if ( itMode == SupportedAwbModes.end( ) )
        {
            ret = XError::InvalidPropertyValue;
        }
        else
        {
            propStatus = mCamera->SetWhiteBalanceMode( itMode->second );
        }
    }
    else if ( propertyName == PROP_EXPMODE )
    {
        map<string, ExposureMode>::const_iterator itMode = SupportedExposureModes.find( value );
        
        if ( itMode == SupportedExposureModes.end( ) )
        {
            ret = XError::InvalidPropertyValue;
        }
        else
        {
            propStatus = mCamera->SetExposureMode( itMode->second );
        }
    }
    else if ( propertyName == PROP_EXPMETERINGMODE )
    {
        map<string, ExposureMeteringMode>::const_iterator itMode = SupportedExposureMeteringModes.find( value );
        
        if ( itMode == SupportedExposureMeteringModes.end( ) )
        {
            ret = XError::InvalidPropertyValue;
        }
        else
        {
            propStatus = mCamera->SetExposureMeteringMode( itMode->second );
        }            
    }
    else if ( propertyName == PROP_EFFECT )
    {
        map<string, ImageEffect>::const_iterator itMode = SupportedImageEffects.find( value );
        
        if ( itMode == SupportedImageEffects.end( ) )
        {
            ret = XError::InvalidPropertyValue;
        }
        else
        {
            propStatus = mCamera->SetImageEffect( itMode->second );
        }            
    }
    else
    {
        ret = XError::UnknownProperty;
    }

    if ( ( ret == XError::Success ) && ( !propStatus ) )
    {
        ret = XError::Failed;
    }

    return ret;
}

// Get the specified property of a PI camera
XError XRaspiCameraConfig::GetProperty( const string& propertyName, string& value ) const
{
    XError ret = XError::Success;
    bool   valueIsANumber = false;
    int    numericValue;

    if ( propertyName == PROP_HFLIP )
    {
        value = ( mCamera->GetHorizontalFlip( ) ) ? "1" : "0";
    }
    else if ( propertyName == PROP_VFLIP )
    {
        value = ( mCamera->GetVerticalFlip( ) ) ? "1" : "0";
    }
    else if ( propertyName == PROP_VIDEO_STABILISATION )
    {
        value = ( mCamera->GetVideoStabilisation( ) ) ? "1" : "0";
    }
    else if ( propertyName == PROP_SHARPNESS )
    {
        numericValue   = mCamera->GetSharpness( );
        valueIsANumber = true;
    }
    else if ( propertyName == PROP_CONTRAST )
    {
        numericValue   = mCamera->GetContrast( );
        valueIsANumber = true;
    }
    else if ( propertyName == PROP_BRIGHTNESS )
    {
        numericValue   = mCamera->GetBrightness( );
        valueIsANumber = true;
    }
    else if ( propertyName == PROP_SATURATION )
    {
        numericValue   = mCamera->GetSaturation( );
        valueIsANumber = true;
    }
    else if ( propertyName == PROP_AWBMODE )
    {
        AwbMode awbMode = mCamera->GetWhiteBalanceMode( );

        for ( auto supportedMode : SupportedAwbModes )
        {
            if ( supportedMode.second == awbMode )
            {
                value = supportedMode.first;
                break;
            }
        }
    }
    else if ( propertyName == PROP_EXPMODE )
    {
        ExposureMode expMode = mCamera->GetExposureMode( );

        for ( auto supportedMode : SupportedExposureModes )
        {
            if ( supportedMode.second == expMode )
            {
                value = supportedMode.first;
                break;
            }
        }
    }
    else if ( propertyName == PROP_EXPMETERINGMODE )
    {
        ExposureMeteringMode expMeteringMode = mCamera->GetExposureMeteringMode( );

        for ( auto supportedMode : SupportedExposureMeteringModes )
        {
            if ( supportedMode.second == expMeteringMode )
            {
                value = supportedMode.first;
                break;
            }
        }
    }
    else if ( propertyName == PROP_EFFECT )
    {
        ImageEffect imageEffect = mCamera->GetImageEffect( );

        for ( auto supportedEffect : SupportedImageEffects )
        {
            if ( supportedEffect.second == imageEffect )
            {
                value = supportedEffect.first;
                break;
            }
        }
    }
    else
    {
        ret = XError::UnknownProperty;
    }

    if ( valueIsANumber )
    {
        char buffer[32];

        sprintf( buffer, "%d", numericValue );
        value = buffer;
    }

    return ret;
}

// Get all supported properties of a PI camera
map<string, string> XRaspiCameraConfig::GetAllProperties( ) const
{
    map<string, string> properties;

    for ( int i = 0; i < sizeof( SupportedProperties ) / sizeof( SupportedProperties[0] ); i++ )
    {
        string value;

        if ( GetProperty( *SupportedProperties[i], value ) == XError::Success )
        {
            properties.insert( pair<string, string>( *SupportedProperties[i], value ) );
        }
    }

    return properties;
}

